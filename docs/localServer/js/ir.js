
function sendRawSignal(){
   var signal =  $("#sig1").val() + $("#sig2").val()+ $("#sig3").val()+ $("#sig4").val();
   robotIp = $("#selectIP").val();

   $.ajax({
      type: "GET",
      dataType: "json",
      url: "http://" + robotIp + "/ir/send?signal="+signal,

      success: function (data) {
         var response = data;
         console.log(response);
         return true;
      },
      error: function (request, status, err) {
         console.error(request + " " + status);
         return false;
      },

      timeout: 1000
   });

   return false;
}
