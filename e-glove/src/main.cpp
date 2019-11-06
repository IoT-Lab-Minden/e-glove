#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <string>
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"
bool connected = false;
  BLEService *pService;
  BLEService *pService1;
  BLECharacteristic *reportChar1;
  BLECharacteristic *reportChar2;
  BLECharacteristic *reportChar3;
  void setupCharacteristics();
  class MyCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer){
      connected=true;
    }
    
    void onDisconnect(BLEServer* pServer){
      connected=false;
    }
  };
  
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("E-Glove");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());
  pService = pServer->createService((uint16_t)0x180a);
  pService1 = pServer->createService((uint16_t)0x1812, 30);
  setupCharacteristics();

  pService->start();
  pService1->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID((uint16_t)0x1812);
  pAdvertising->start();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
uint8_t v[] = {0x0, 0x0, 0x0, 0x0,0x0,0x0,0x0,0x0};

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  if(connected){
    uint8_t a[] = {0x0, 0x0, random(0x04,0x26), 0x0,0x0,0x0,0x0,0x0};
    reportChar1->setValue(a,sizeof(a));
    reportChar1->notify();    

    reportChar1->setValue(v, sizeof(v));
    reportChar1->notify();
  }
}

void setupCharacteristics() {

  BLECharacteristic *manufacturer = pService->createCharacteristic(
                                       (uint16_t)0x2a29,
                                       BLECharacteristic::PROPERTY_READ 
                                     );
  std::string name = "espressif";
  manufacturer->setValue(name);
                                     
  BLECharacteristic *pnpIDChar = pService->createCharacteristic(
                                       (uint16_t)0x2a50,
                                       BLECharacteristic::PROPERTY_READ 
                                     );
  const uint8_t pnp[] = {0x01,0xe5,0x02,0xcd,0xab,0x01,0x00};
  pnpIDChar->setValue((uint8_t*)pnp, sizeof(pnp));
  
  BLECharacteristic *hidInfoChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a4a,
                                       BLECharacteristic::PROPERTY_READ 
                                     );
  const uint8_t val1[] = {0x00,0x01,0x00,0x02};
  hidInfoChar->setValue((uint8_t*)val1, 4);

  BLECharacteristic *reportMapChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a4b,
                                       BLECharacteristic::PROPERTY_READ 
                                     );
  const uint8_t val2[] = {
    0x05,0x01,0x09,0x06,0xA1,0x01,0x05,0x07,
    0x19,0xE0,0x29,0xE7,0x15,0x00,0x25,0x01,
    0x75,0x01,0x95,0x08,0x81,0x02,0x95,0x01,
    0x75,0x08,0x81,0x01,0x95,0x05,0x75,0x01,
    0x05,0x08,0x19,0x01,0x29,0x05,0x91,0x02,
    0x95,0x01,0x75,0x03,0x91,0x01,0x95,0x06,
    0x75,0x08,0x15,0x00,0x25,0x65,0x05,0x07,
    0x19,0x00,0x29,0x65,0x81,0x00,0xC0}; //TODO
  reportMapChar->setValue((uint8_t*)val2, sizeof(val2));

  BLECharacteristic *hidControlChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a4c,
                                       BLECharacteristic::PROPERTY_WRITE_NR 
                                     );

  reportChar1 = pService1->createCharacteristic(
                                       (uint16_t)0x2a4d,
                                       BLECharacteristic::PROPERTY_READ   |
                                       BLECharacteristic::PROPERTY_NOTIFY                                       
                                     );
  BLEDescriptor *desc1 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
  const uint8_t desc1_val[] = {0x01, 0};
  desc1->setValue((uint8_t*)desc1_val, 1);
  reportChar1->addDescriptor(desc1);
  reportChar1->addDescriptor(new BLE2902());

  reportChar2 = pService1->createCharacteristic(
                                       (uint16_t)0x2a4d,
                                       BLECharacteristic::PROPERTY_READ   |
                                       BLECharacteristic::PROPERTY_WRITE                                       
                                     );
  BLEDescriptor *desc2 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
  const uint8_t desc2_val[] = {0x02, 0};
  desc2->setValue((uint8_t*)desc2_val, 1);
  reportChar2->addDescriptor(desc2);

  reportChar3 = pService1->createCharacteristic(
                                       (uint16_t)0x2a4d,
                                       BLECharacteristic::PROPERTY_READ     |
                                       BLECharacteristic::PROPERTY_WRITE    |
                                       BLECharacteristic::PROPERTY_WRITE_NR 
                                     );
  BLEDescriptor *desc3 = new BLEDescriptor(BLEUUID((uint16_t)0x2908));
  const uint8_t desc3_val[] = {0x03, 0};
  desc3->setValue((uint8_t*)desc3_val, 1);
  reportChar3->addDescriptor(desc3);

  BLECharacteristic *protocolModeChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a4e,
                                       BLECharacteristic::PROPERTY_WRITE_NR 
                                     );

  BLECharacteristic *bootInputChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a22,
                                       BLECharacteristic::PROPERTY_NOTIFY
                                     );
  bootInputChar->addDescriptor(new BLE2902());


  BLECharacteristic *bootOutputChar = pService1->createCharacteristic(
                                       (uint16_t)0x2a32,
                                       BLECharacteristic::PROPERTY_READ     |
                                       BLECharacteristic::PROPERTY_WRITE    |
                                       BLECharacteristic::PROPERTY_WRITE_NR 
                                     );



}