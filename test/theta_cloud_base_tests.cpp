#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "DeviceHandlerToken.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "fakeIncludes/generic_mocks.hpp"

using ::testing::_;
using ::testing::Invoke;

const auto TEST_SENSOR_DATA = SensorData{"TEST_TOPIC", "EXAMPLE_WRITTEN_VALUE"};

struct ThetaCloudWithoutDataCallbackFixture : public GenericTest {
protected:
    MockCallback mockCallback;
    ThetaCloud testedThetaCloud;
};

struct ThetaCloudFixture : public ThetaCloudWithoutDataCallbackFixture {
protected:
    ThetaCloudFixture()
    {
        testedThetaCloud.whenDataAvailable([this](const SensorData &data) {
            mockCallback.callback(data);
        });
    }
};

struct ThetaCloudWithExampleReadHandler : public ThetaCloudFixture {
    MockReadHandler mockReadHandler;
    ThetaCloudWithExampleReadHandler()
    {
        readHandlerToken = testedThetaCloud.addReadHandler(
            [this](const ThetaCloud::Emit &emit) {
            mockReadHandler.readHandler(emit);
        });
    }

    DeviceHandlerTokenPtr readHandlerToken;
};

struct ThetaCloudWithExampleWriteHandler : public ThetaCloudFixture {
    const std::string TEST_TOPIC = "test topic";
    MockWriteHandler mockWriteHandler;
    ThetaCloudWithExampleWriteHandler()
    {
        writeHandlerToken = testedThetaCloud.addWriteHandler(TEST_TOPIC,
                                                             [this](const SensorData &data,
                                                                    const ThetaCloud::Emit &emit) {
            mockWriteHandler.writeHandler(data, emit);
        });
    }

    DeviceHandlerTokenPtr writeHandlerToken;
};

TEST_F(ThetaCloudWithExampleReadHandler, ReadCallbacksNotCalledWhenNotInitialized) {
    testedThetaCloud.tick();
}

TEST_F(ThetaCloudWithoutDataCallbackFixture, ReceivingDataWithoutDataHandlerDoesNotDoAnything) {
    auto readHandlerToken = testedThetaCloud.addReadHandler(
        [this](const ThetaCloud::Emit &e) {
        e(TEST_SENSOR_DATA);
    });
    testedThetaCloud.tick();
}

TEST_F(ThetaCloudWithExampleReadHandler, ReadCallbacksCalledWhenInitialized) {
    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    EXPECT_CALL(mockReadHandler, readHandler(_)).WillOnce(Invoke(
                                                              [this](const ThetaCloud::Emit &e){
        e(TEST_SENSOR_DATA);
    }));
    EXPECT_CALL(mockCallback, callback(TEST_SENSOR_DATA));
    testedThetaCloud.tick();
}

TEST_F(ThetaCloudWithExampleReadHandler, EmitMethodPublishesTheDataImmediately) {
    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    EXPECT_CALL(mockCallback, callback(TEST_SENSOR_DATA));
    testedThetaCloud.emit(TEST_SENSOR_DATA);
}

TEST_F(ThetaCloudFixture, NothingHappensWhenThereAreNoReadCallbacks) {
    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    testedThetaCloud.tick();
}

TEST_F(ThetaCloudFixture, ServicesCanUnsubscribeByDeletingToken) {
    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    MockReadHandler mockReadHandlerAlwaysCalled;
    auto token1 = testedThetaCloud.addReadHandler(
        [&](const ThetaCloud::Emit &emit) {
        mockReadHandlerAlwaysCalled.readHandler(emit);
    });
    EXPECT_CALL(mockReadHandlerAlwaysCalled, readHandler(_));
    testedThetaCloud.tick();
    {
        MockReadHandler mockReadHandlerOnceCalled;
        auto token2 = testedThetaCloud.addReadHandler(
            [&](const ThetaCloud::Emit &emit) {
            mockReadHandlerOnceCalled.readHandler(emit);
        });
        EXPECT_CALL(mockReadHandlerAlwaysCalled, readHandler(_));
        EXPECT_CALL(mockReadHandlerOnceCalled, readHandler(_));
        testedThetaCloud.tick();
    }
    EXPECT_CALL(mockReadHandlerAlwaysCalled, readHandler(_));
    testedThetaCloud.tick();
}

TEST_F(ThetaCloudWithExampleWriteHandler, WritingCausesAnAction) {
    const std::string EXAMPLE_WRITTEN_VALUE("example value");
    const auto dataToBeSent = SensorData{TEST_TOPIC, EXAMPLE_WRITTEN_VALUE};

    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    EXPECT_CALL(mockCallback, callback(dataToBeSent));
    EXPECT_CALL(mockWriteHandler, writeHandler(dataToBeSent, _)).WillOnce(Invoke(
                                                                              [this](const
                                                                                     SensorData &
                                                                                     data,
                                                                                     const
                                                                                     ThetaCloud::
                                                                                     Emit &emit){
        emit(data);
    }));
    testedThetaCloud.write(dataToBeSent);
}

TEST_F(ThetaCloudWithExampleWriteHandler, DoubleSubscribeDoesntDoAnything) {
    const std::string EXAMPLE_WRITTEN_VALUE("example value");
    const auto dataToBeSent = SensorData{TEST_TOPIC, EXAMPLE_WRITTEN_VALUE};
    MockWriteHandler secondMockWriteHandler;
    EXPECT_CALL(secondMockWriteHandler, writeHandler(_, _)).Times(0);

    auto secondWriteHandlerToken = testedThetaCloud.addWriteHandler(TEST_TOPIC,
                                                                    [&](const SensorData &data,
                                                                        const ThetaCloud::Emit &emit) {
        secondMockWriteHandler.writeHandler(data, emit);
    });

    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    EXPECT_CALL(mockCallback, callback(dataToBeSent));
    EXPECT_CALL(mockWriteHandler, writeHandler(dataToBeSent, _)).WillOnce(Invoke(
                                                                              [this](const
                                                                                     SensorData &
                                                                                     data,
                                                                                     const
                                                                                     ThetaCloud::
                                                                                     Emit &emit){
        emit(data);
    }));
    testedThetaCloud.write(dataToBeSent);
}

TEST_F(ThetaCloudWithExampleWriteHandler, Resubscription) {
    const std::string EXAMPLE_WRITTEN_VALUE("example value");
    const std::string OTHER_WRITTEN_VALUE("other value");
    const auto dataToBeSent = SensorData{TEST_TOPIC, EXAMPLE_WRITTEN_VALUE};
    const auto otherDataToBeSent = SensorData{TEST_TOPIC, OTHER_WRITTEN_VALUE};

    EXPECT_CALL((*Wire.mock), begin(2, 14)).Times(1);
    testedThetaCloud.init();
    EXPECT_CALL(mockCallback, callback(dataToBeSent));
    EXPECT_CALL(mockWriteHandler, writeHandler(dataToBeSent, _)).WillOnce(Invoke(
                                                                              [this](const
                                                                                     SensorData &
                                                                                     data,
                                                                                     const
                                                                                     ThetaCloud::
                                                                                     Emit &emit){
        emit(data);
    }));
    testedThetaCloud.write(dataToBeSent);

    MockWriteHandler secondMockWriteHandler;
    writeHandlerToken.reset();  // unsubscribe an old handler
    auto secondWriteHandlerToken = testedThetaCloud.addWriteHandler(TEST_TOPIC,
                                                                    [&](const SensorData &data,
                                                                        const ThetaCloud::Emit &emit) {
        secondMockWriteHandler.writeHandler(data, emit);
    });
    EXPECT_CALL(secondMockWriteHandler, writeHandler(otherDataToBeSent, _)).WillOnce(Invoke(
                                                                                         [this](
                                                                                             const
                                                                                             SensorData
                                                                                             &data,
                                                                                             const
                                                                                             ThetaCloud
                                                                                             ::Emit
                                                                                             &emit){
        emit(data);
    }));
    EXPECT_CALL(mockCallback, callback(otherDataToBeSent));
    testedThetaCloud.write(otherDataToBeSent);
}
