var connection=false;

function connect() {
   $(".tablink").removeClass("w3-disabled");
   $(".cmdBtn").removeClass("w3-disabled");

   connection=true;
   robotIp = $("#selectIP").val();

   $.ajax({
      type: "GET",
      dataType: "json",
      url: "http://" + robotIp + "/status",
      success: function (data) {

         var response = data;

         var robotVersion = response.version;
         var robotStatus = response.status;
         var robotSerial = response.serial;
         var robotAuthor = response.author;

         console.log(215165);

         if(robotStatus == "Active"){
            $("#txtStatus").val(robotStatus);

            $("#txtRobotId").html("0");
            $("#txtVersion").html(robotVersion);
            $("#txtSerial").html(robotSerial);
            $("#txtAuthor").html(robotAuthor);
            $("#txtRobotFeatures").html("N/A");

            console.log("Connection:" + robotIp + " "+ robotStatus + "(" + robotVersion+ ")");
            return true;

         }else{
            return false;
         }

      },
      error: function (request, status, err) {
         console.error(request + " " + status);

         $("#txtrobotId").val(0);
         $("#txtStatus").val("Not Connected");
         $("#txtVersion").html("N/A");
         $("#txtSerial").html("N/A");
         $("#txtAuthor").html("N/A");

         return false;
      },
      timeout: 1000
   });

   return false;
}
function networkSearch(){

   var subnet = "192.168.43.";
   var from = 1;
   var to = 254;
   var d = 0;      // No of active devices

   $("#selectIP").html("<option value=\""+subnet+"0\" disabled selected>None</option>"); // clear the select box

   for(i=from;i<=to;i++){
      robotIp = subnet + i;

      $.ajax({
         type: "GET",
         dataType: "json",
         url: "http://" + robotIp + "/status",
         success: function (data) {
            var response = data;
            robotVersion = response.version;
            robotStatus = response.status;
            $("#selectIP").append("<option value='"+ response.IP +"'>"+  response.IP + "</option>").val(response.IP);
            d = d + 1;
         },
         error: function (request, status, err) {
            // Nothing to do here
         },
         timeout: 1500
      });
   }
}
