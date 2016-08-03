#include <opalKellyAtisSepia.hpp>
#include <tarsier/stitch.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/logarithmicDisplay.hpp>
#include <QGuiApplication>
#include <QtQuick/QQuickView>

/// ExposureMeasurement holds the parameters of an exposure measurement.
struct ExposureMeasurement {
    size_t x;
    size_t y;
    uint64_t timeDelta;
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::LogarithmicDisplay>("LogarithmicDisplay", 1, 0, "LogarithmicDisplay");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto logarithmicDisplay = view.rootObject()->findChild<chameleon::LogarithmicDisplay*>("logarithmicDisplay");

    auto camera = opalKellyAtisSepia::make_camera(
        sepia::make_split(
            [changeDetectionDisplay](sepia::ChangeDetection changeDetection) -> void {
                changeDetectionDisplay->push(changeDetection);
            },
            tarsier::make_stitch<sepia::ThresholdCrossing, ExposureMeasurement>(
                opalKellyAtisSepia::Camera::width(),
                opalKellyAtisSepia::Camera::height(),
                [](sepia::ThresholdCrossing secondThresholdCrossing, uint64_t timeDelta) -> ExposureMeasurement {
                    return ExposureMeasurement{secondThresholdCrossing.x, secondThresholdCrossing.y, timeDelta};
                },
                [logarithmicDisplay](ExposureMeasurement exposureMeasurement) -> void {
                    logarithmicDisplay->push(exposureMeasurement);
                }
            )
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
