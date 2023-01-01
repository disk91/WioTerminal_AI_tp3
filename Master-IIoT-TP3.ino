/* ****************************************************
 *  PRACTICE - Topic #3
 * ----------------------------------------------------
 * Use a camera to count pieces passing through the camera
 * An AI algorithm trained as indicated in the associated 
 * blog post is detecting BLUE and RED poker token
 */

#include "Seeed_Arduino_GroveAI.h"
#include <Wire.h>

GroveAI ai = GroveAI(Wire);

void setup() {
  Wire.begin();  
  Serial.begin(115200);
  while ( !Serial );
  Serial.print("Go");
  for (int i = 0 ; i < 6 ; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  
  if ( ! ai.begin(ALGO_OBJECT_DETECTION, MODEL_EXT_INDEX_1))  {
    Serial.println("Failed to find camera");
    while(true);
  }
}


void loop() {

 // start processing and wait for end
 if ( ai.invoke() ) {
   while ( ai.state() != CMD_STATE_IDLE);
   uint8_t len = ai.get_result_len();
   if ( len > 0 ) {
     // some result found
     object_detection_t data; 
     // find best accuracy to only display this one
     int high = 0, ihigh=-1;
     for ( int i = 0 ; i < len ; i++ ) {
        ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t)); 
        if ( data.confidence > high ) { high = data.confidence; ihigh = i; }
     }
     ai.get_result(ihigh, (uint8_t*)&data, sizeof(object_detection_t)); 
        
     if ( data.confidence > 80 ) {
       int t = data.target;
       Serial.print("Detected target: ");
       switch ( data.target ) {
         case 0 : Serial.print("Blue Token");
           break;
         case 1 : Serial.print("Empty");
           break;
         case 2 : Serial.print("Red Token");
           break;
       }
       Serial.print(" with accuracy ");
       Serial.print(data.confidence);
       Serial.println("%"); 
     }
       
   } 
 } else delay(2000);
}
