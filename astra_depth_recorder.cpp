#include <chrono>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <astra/astra.hpp>
#include <astra_core/astra_core.hpp>

// Camera parameters
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;
constexpr int HEIGHT_DEPTH = 400;
constexpr int FPS = 30;


astra::DepthStream configure_depth(astra::StreamReader &reader) {
    auto depthStream = reader.stream<astra::DepthStream>();

    //We don't have to set the mode to start the stream, but if you want to here is how:
    astra::ImageStreamMode depthMode;

    depthMode.set_width(WIDTH);
    depthMode.set_height(HEIGHT_DEPTH);
    depthMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_DEPTH_MM);
    depthMode.set_fps(FPS);

    depthStream.set_mode(depthMode);
    return depthStream;
}


astra::ColorStream configure_color(astra::StreamReader &reader) {
    auto colorStream = reader.stream<astra::ColorStream>();

    astra::ImageStreamMode colorMode;
    colorMode.set_width(WIDTH);
    colorMode.set_height(HEIGHT);
    colorMode.set_pixel_format(astra_pixel_formats::ASTRA_PIXEL_FORMAT_RGB888);
    colorMode.set_fps(FPS);

    colorStream.set_mode(colorMode);
    return colorStream;
}


int main(int argc, char *argv[]) {
    if (argc != 2) { // We expect 3 arguments: the program name, the source path and the destination path
        std::cerr << "Usage: " << argv[0] << "DEST_DIR" << std::endl;
        return 1;
    }

    // Setup camera
    astra::initialize();

    astra::StreamSet streamSet1("device/sensor0");
    astra::StreamReader reader = streamSet1.create_reader();
    auto depthStream = configure_depth(reader);
    auto colorStream = configure_color(reader);
    depthStream.start();
    colorStream.start();
    depthStream.enable_registration(true);

    int n = 0;
    while (reader.is_valid())
    {
        astra_update();

        auto frame = reader.get_latest_frame();
        if (!frame.is_valid()) {
            continue;
        }
        auto colorFrame = frame.get<astra::ColorFrame>();
        if (!colorFrame.is_valid()) {
            continue;
        }
        auto depthFrame = frame.get<astra::DepthFrame>();
        if (!depthFrame.is_valid()) {
            continue;
        }
        cv::Mat depth(HEIGHT_DEPTH, WIDTH, CV_16UC1, (uint16_t *) depthFrame.data());
        cv::Mat rgb(HEIGHT, WIDTH, CV_8UC3, (uint8_t *) colorFrame.data());
        cv::cvtColor(rgb, rgb, cv::COLOR_BGR2RGB);

        // Save
        std::stringstream filename;
        filename.fill('0');
        filename.width(5);
        filename<<std::to_string(n);
        cv::imwrite(argv[1] + filename.str() + "_rgb.png", rgb);
        cv::imwrite(argv[1] + filename.str() + "_depth.png", depth);
        ++n;

        // Display
        cv::copyMakeBorder(depth, depth, 0, HEIGHT - HEIGHT_DEPTH, 0, 0, cv::BORDER_CONSTANT, 0);
        cv::normalize(depth, depth, 255, 0, cv::NORM_MINMAX);
        cv::Mat normalized_depth(depth);
        cv::Mat gray_image;
        normalized_depth.convertTo(normalized_depth, CV_8U);
        cv::cvtColor(normalized_depth, gray_image, cv::COLOR_GRAY2BGR);
        cv::Mat overlay;
        cv::addWeighted(rgb, 0.6, gray_image, 0.8, 0, overlay);

        cv::imshow("RGB", rgb);
        cv::imshow("overlay", overlay);
        cv::imshow("depth", depth * 20);

        int k = cv::waitKey(1);
        if (k == 8 || k == 27 || k == 32 || k == 113) {
            // 8 = ascii value of BACKSPACE
            // 27 = ascii value of ESC
            // 32 = ascii value of SPACE
            // 113 = ascii value of `q`
            break;
        }
    }

    cv::destroyAllWindows();
    astra::terminate();
    std::cout << "Terminated" << std::endl;
    return 0;
}
