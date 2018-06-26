#include "../third_party/chameleon/source/background_cleaner.hpp"
#include "../third_party/chameleon/source/delta_t_display.hpp"
#include "../third_party/chameleon/source/dvs_display.hpp"
#include "../third_party/chameleon/source/frame_generator.hpp"
#include "../third_party/sepia/source/sepia.hpp"
#include "../third_party/tarsier/source/replicate.hpp"
#include "../third_party/tarsier/source/stitch.hpp"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join(
    {sepia::dirname(SEPIA_DIRNAME), "third_party", "sepia", "third_party", "event_stream", "examples", "atis.es"});

/// exposure_measurement holds the parameters of an exposure measurement.
SEPIA_PACK(struct exposure_measurement {
    uint64_t delta_t;
    uint16_t x;
    uint16_t y;
});

/// frame_duration is the time between two generated frames in microseconds.
const uint64_t frame_duration = 50000;

int main(int argc, char* argv[]) {
    // read the header
    const auto header = sepia::read_header(sepia::filename_to_ifstream(filename));

    // create the Qt Application
    QGuiApplication app(argc, argv);

    // register Chameleon types
    qmlRegisterType<chameleon::background_cleaner>("Chameleon", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::dvs_display>("Chameleon", 1, 0, "DvsDisplay");
    qmlRegisterType<chameleon::delta_t_display>("Chameleon", 1, 0, "DeltaTDisplay");
    qmlRegisterType<chameleon::frame_generator>("Chameleon", 1, 0, "FrameGenerator");

    // pass the header properties to qml
    QQmlApplicationEngine application_engine;
    application_engine.rootContext()->setContextProperty("header_width", header.width);
    application_engine.rootContext()->setContextProperty("header_height", header.height);

    // load the view and setup the window properties for OpenGL renderring
    application_engine.loadData(
#include "events_to_frames.qml"
    );
    auto window = qobject_cast<QQuickWindow*>(application_engine.rootObjects().first());
    {
        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
        window->setFormat(format);
    }

    // retrieve pointers to the displays generated by qml
    auto dvs_display = window->findChild<chameleon::dvs_display*>("dvs_display");
    auto delta_t_display = window->findChild<chameleon::delta_t_display*>("delta_t_display");
    auto frame_generator = window->findChild<chameleon::frame_generator*>("frame_generator");

    // create the event handling pipeline
    std::size_t frame_index = 0;
    auto observable = sepia::make_observable<sepia::type::atis>(
        sepia::filename_to_ifstream(filename),
        tarsier::make_replicate<sepia::atis_event>(
            [&](sepia::atis_event atis_event) {
                if (atis_event.t > frame_index * frame_duration) {
                    frame_generator->save_frame_to(std::to_string(frame_index) + ".png");
                    ++frame_index;
                }
            },
            sepia::make_split<sepia::type::atis>(
                [&](sepia::dvs_event dvs_event) { dvs_display->push(dvs_event); },
                tarsier::make_stitch<sepia::threshold_crossing, exposure_measurement>(
                    header.width,
                    header.height,
                    [](sepia::threshold_crossing threshold_crossing, uint64_t delta_t) -> exposure_measurement {
                        return {delta_t, threshold_crossing.x, threshold_crossing.y};
                    },
                    [&](exposure_measurement exposure_measurement) { delta_t_display->push(exposure_measurement); }))),
        [&](std::exception_ptr) { app.quit(); });

    // run the Qt Application
    return app.exec();
}
