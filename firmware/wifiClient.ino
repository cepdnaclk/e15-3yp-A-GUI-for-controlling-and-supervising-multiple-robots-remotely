
#ifdef ENABLE_WIFI_CLIENT

int httpGET(String URL, String parameters){

   URL.concat(parameters);

   if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(URL); // Ex: "http://68.183.188.135:8080/test"
      http.addHeader("Content-Type", "text/plain");

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
         String response = http.getString();
         Serial.println(httpResponseCode);   //Print return code
         Serial.println(response);           //Print request answer

      } else {
         Serial.printf("Error on sending GET: %d \n", httpResponseCode);
      }

      http.end();
      return httpResponseCode;
   }
   else {
      Serial.println("Error in WiFi connection");
      delay(5000);
      return 0;
   }
}
int httpPOST(String URL){

   if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin(URL); // Ex: "http://68.183.188.135:8080/test"
      http.addHeader("Content-Type", "text/plain");

      // Add post headers
      http.addHeader("postEn", "true");

      int httpResponseCode = http.POST("");

      if (httpResponseCode > 0) {
         String response = http.getString();
         Serial.println(httpResponseCode);   //Print return code
         Serial.println(response);           //Print request answer

      } else {
         Serial.printf("Error on sending GET: %d \n", httpResponseCode);
      }

      http.end();
      return httpResponseCode;
   }
   else {
      Serial.println("Error in WiFi connection");
      delay(5000);
      return 0;
   }
}

#else

int httpGET(String URL){
   return -1;
}
int httpPOST(String URL){
   return -1;
}

#endif
