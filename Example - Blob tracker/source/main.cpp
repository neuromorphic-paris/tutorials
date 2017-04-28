#include <sepia.hpp>
#include <tarsier/maskIsolated.hpp>
#include <tarsier/replicate.hpp>
#include <tarsier/trackBlobs.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/blobDisplay.hpp>
#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::BlobDisplay>("BlobDisplay", 1, 0, "BlobDisplay");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QQuickView view;
    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("../../source/main.qml"));
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto blobDisplay = view.rootObject()->findChild<chameleon::BlobDisplay*>("blobDisplay");
    auto hiddenBlobDisplay = view.rootObject()->findChild<chameleon::BlobDisplay*>("hiddenBlobDisplay");

    auto atisEventStreamObservable = sepia::make_atisEventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        sepia::make_split(
            tarsier::make_maskIsolated<sepia::DvsEvent, 304, 240, 10000>(
                tarsier::make_replicate<sepia::DvsEvent>(
                    tarsier::make_trackBlobs<sepia::DvsEvent>(
                        {
                            tarsier::Blob{38 * 1, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 1, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 1, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 5, 250, 0, 250},
                        },
                          1e5, // activityDecay
                            0, // minimumProbability
                            1, // promotionActivity
                          0.1, // deletionActivity
                         0.99, // meanInertia
                         0.99, // covarianceInertia
                            1, // repulsionStrength
                           20, // repulsionLength
                         0.05, // attractionStrength
                          100, // attractionResetDistance
                        10000, // pairwiseCalculationsToSkip
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->promoteBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->updateBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->demoteBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->promoteBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->updateBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->demoteBlob(id, blob);
                        },
                        [&](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->deleteBlob(id, blob);
                            hiddenBlobDisplay->deleteBlob(id, blob);
                        }
                    ),
                    [&](sepia::DvsEvent dvsEvent) -> void {
                        changeDetectionDisplay->push(dvsEvent);
                    }
                )
            ),
            [](sepia::ThresholdCrossing) -> void {}
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
