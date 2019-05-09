#include "IFCamera.h"
#include "SquareMap.h"


IFCamera::IFCamera() {};

void IFCamera::initSetup() {
  Serial.println("Setting up camera");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_YUV422;//JPEG;//PIXFORMAT_GRAYSCALE;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 0);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);
  millisPast = millis();
  Serial.println("End setting up camera");

  //udpToMicroMatrix = new UDPCommunication ("10.10.10.3", 5678);
  //udpToPC = new UDPCommunication ("10.10.10.2", 5678);
}

void IFCamera::setThreshold(int thresholdValue) {
  _thresholdValue = thresholdValue;
}

void IFCamera::readNProcess(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
  auto fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
  } else {
    Serial.printf("Capturing camera a %.1f fps with %d x %d framesize and buffer lenght %d \n", fps(), fb->width, fb->height, fb->len);
    auto frameBufferIn = fb->buf;
    int frameBufferGraySize = fb->width * fb->height;
    auto frameBufferGray = new uint8_t[frameBufferGraySize];
    yuvToGrayscale(fb->width, fb->height, frameBufferIn, frameBufferGray);
    SquareMap select_map(frameBufferGray, fb->width, fb->height);
    auto select_square = select_map.get_square(40, 0, 240, 240);
    //udpToMicroMatrix->send((uint8_t*)select_square->getValues(), 128 * 128);
    //udpToPC->send((uint8_t*)select_square->getValues(), 128 * 128);
    auto reduce_square = select_square->reduce(8, 8);
    memcpy(grayArray, reduce_square->getValues(), 64);
    //frameBufferToArrayMod(fb->width, fb->height, frameBufferGray, grayArray);
    //delete frameBufferGray;
    delete select_square;
    delete reduce_square;
  }
  esp_camera_fb_return(fb);
  fb = NULL;
}

float IFCamera::fps() {
  int framePeriod = millis() - millisPast;
  millisPast = millis();
  return 1.0 / (float(framePeriod) / 1000.0);
}

void IFCamera::yuvToGrayscale(int width, int height, uint8_t* frameBufferInput, uint8_t* frameBufferGray) {
  int outIndex = 0;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width * 2; i += 2) {
      frameBufferGray[outIndex] = frameBufferInput[(j * width * 2) + i];
      outIndex++;
    }
  }
  //Serial.printf("input size: %d, output size: %d \n", width * height, outIndex);
}

void IFCamera::frameBufferToArrayMod(int width, int height, uint8_t* frameBufferGray, uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
  int index = 0;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (i % int(width / X_SIZE) == 0 && j % int(height / Y_SIZE) == 0) {
        grayArray[index] = frameBufferGray[i + j * width];
        index++;
      }
    }
  }
}


void IFCamera::printGrayMatrix(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
  for (int j = 0; j < Y_SIZE; j++) {
    for (int i = 0; i < X_SIZE; i++) {
      Serial.printf("%d ", grayArray[i + j * X_SIZE]);
    }
    Serial.println();
  }
  Serial.println("\n");
}

void IFCamera::printBinaryMatrix(uint8_t (& grayArray) [X_SIZE * Y_SIZE]) {
  for (int j = 0; j < Y_SIZE; j++) {
    for (int i = 0; i < X_SIZE; i++) {
      if (grayArray[i + j * X_SIZE] >= _thresholdValue) Serial.print("O");
      else Serial.print("X");
    }
    Serial.println();
  }
  Serial.println("\n");
}
