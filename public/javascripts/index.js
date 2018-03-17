/* global palette */
/* global $ */
/* global Chart */

$(document).ready(function () {
  window.isConnected = 0;

  // https://hometempfunctionapp.azurewebsites.net/api/oneHourAgo?period=hour
  // https://hometempfunctionapp.azurewebsites.net/api/oneDayAgo?period=day

  $.getJSON("sensors.json", function(config) {
    console.log(config); // this will show the info it in firebug console
    
    window.data = config;
    
    loadSensors(config);
    
    connectWebSocket();
  })
  .fail(function( jqxhr, textStatus, error ) {
    var err = textStatus + ", " + error;
    console.warning( "Failed to get configuration file: " + err );
    displayConnectionInfo("error", 'Failed to get configuration file: ' + err);
  });
  
  setInterval(function() { checkConnection(); }, 1000);
});

function connectWebSocket() {
  var ws = new WebSocket("wss://" + location.host);
  
  ws.onopen = function () {
    console.log("Successfully connect WebSocket");
    displayConnectionInfo("success", 'Successfully connect WebSocket');
    window.isConnected = 1;
  };
  
  ws.onmessage = function (message) {
    console.log("received message" + message.data);
    displayConnectionInfo("good", 'received message');
    
    try {
      var obj = JSON.parse(message.data);
      processMessage(window.data, obj);
      displayConnectionInfo("good", 'message processed');
      
    } catch (err) {
      console.warning(err);
      displayConnectionInfo("warning", 'error processing message: '+ err);
    }
  };

  ws.onclose = onSocketError; 
}

function displayConnectionInfo(level, message) {
  $("#connection").attr('class', level);
  $("#connection").html(message);
}

function displayConnectionStatus(status) {
  $("#connectionStatus").attr('class', status);
}

function checkConnection() {
  if ( window.isConnected == 0 ) { // no status
    // console.log('no status');
    displayConnectionStatus('nostatus');
    return;
  }
  else if ( window.isConnected == 1 ) { // connected
    // console.log('still connected');
    displayConnectionStatus('connected');
    return;    
  }
  else if ( window.isConnected == 2 ) { // disconnected
    // console.error('disconnected');
    displayConnectionStatus('disconnected');
    displayConnectionInfo("warning", "Reconnection in 5s");
    setTimeout(function(){ displayConnectionStatus('connecting'); connectWebSocket(); }, 5000);
  }
}

function loadSensors(config) {
  var sensors = config.sensors;
  var measurements = config.measurements;
  var setup = config.setup;
  var websensors = setup.websensors;
  config.timeData = [];   
  
  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    var label = measurementData.label;
    var axis = measurementData.axis;
    var gtype = measurementData.type;
    var fill = measurementData.fill;
    var colorScheme = measurementData.colorScheme;
    var colorPallet = palette(colorScheme, sensors.length);
    measurementData.cfg = [];

    $.each(sensors, function(sensorNum, sensorInfo) {
      // var sensorId = sensorInfo.id;
      var sensorName = sensorInfo.name;
      var sensorType = sensorInfo.type;
      var sensorDesc = sensorInfo.desc;
      
      if ( sensorType == "web" ) {
          if ( sensorName in websensors ) {
            var webSensorData = websensors[sensorName];
            
            if (
                  ( "className" in webSensorData ) && 
                  ( "apiKey"    in webSensorData ) &&
                  ( "cityId"    in sensorInfo    ) &&
                  ( "interval"  in sensorInfo    )
                ) {
                  
              var webSensorClassName = webSensorData.className;
              var webSensorApiKey = webSensorData.apiKey;
                  
              try {
                var Cls = eval(webSensorClassName);
              } catch (err) {
                console.error(err);
                console.warn("websensor ", sensorInfo, webSensorData, " class not imported");
                throw "websensor " + webSensorClassName + " class not imported";
              }
              
              var webSensorCityId = sensorInfo.cityId;
              var webSensorInst = new Cls(webSensorApiKey);
              
              sensorInfo.caller = function(clbk) { webSensorInst.getById(clbk, webSensorCityId); };
            } else {
              console.warn("websensor ", sensorInfo, webSensorData, " not properly configured in setup");
              return;
            }
          } else {
            console.warn("websensor ", sensorInfo, webSensorData, " not found in setup");
            return;
          }
      }

      
      var rgb = hexToRGB(colorPallet[sensorNum]);
      var mainColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 1.0)";
      var backgroundColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 0.4)";
      
      if ( measumentNum == 0 ) {
        sensorInfo.data = {};
      }
      
      sensorInfo.data[measurementKey] = {
        "fill": fill,
        "type": gtype,
        "yAxisID": axis.id,
        "label": label + " " + sensorName + (sensorDesc ? " ("+sensorDesc+")" : ""),
        borderColor: mainColor,
        pointBoarderColor: mainColor,
        pointHoverBorderColor: mainColor,
        pointHoverBackgroundColor: mainColor,
        backgroundColor: backgroundColor,
        data: []
      };
      
      measurementData.cfg.push( sensorInfo.data[measurementKey] );      
    });
  });

  //https://github.com/chartjs/Chart.js/issues/3666
  var ylabelwidth = 0;
  var xlabelwidth = 0;
  var legendwidth = 0;
  
  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    var measurementLabel = measurementData.label;
    
    //Get the context of the canvas element we want to select

    var $chart = $("<canvas>", { id:"myChart_"+measurementKey, width:setup.graph.width, height:setup.graph.height});
    
    $( "#graphs" ).append( $chart ); 
    
    // var optionsNoAnimation = { animation: false };
    var ctx = document.getElementById("myChart_"+measurementKey).getContext("2d");
    var yaxis = measurementData.axis;
    var xaxis = { 
      display: measumentNum == measurements.length-1,
      type: 'time',
      time: {
        unit: 'minute',
        displayFormats: {
          minute: 'HH:mm'
        }
      },
      distribution: 'linear',
      ticks: {
        source: 'labels'
      }
    };
    
    yaxis.afterFit = function(scaleInstance) { if (ylabelwidth < scaleInstance.width) { ylabelwidth = scaleInstance.width; } else { scaleInstance.width = ylabelwidth; } };
    xaxis.afterFit = function(scaleInstance) { if (xlabelwidth < scaleInstance.width) { xlabelwidth = scaleInstance.width; } else { scaleInstance.width = xlabelwidth; } };
        
    measurementData.chart = new Chart(ctx, {
      "type": setup.graph.type,
      "data": {
        labels: config.timeData,
        datasets: measurementData.cfg
      },
      "options": {
        title: {
          display: setup.graph.displayTitle,
          text: measurementLabel,
          fontSize: setup.graph.titleSize
        },
        scales: {
          yAxes: [yaxis],
          xAxes: [xaxis]
        },
        legend: {
          display: true,
          position: setup.graph.labelPosition,
          afterFit: function(scaleInstance) { if (legendwidth < scaleInstance.width) {legendwidth = scaleInstance.width;} else {legendwidth.width = ylabelwidth;} }
        }
      }
    });
  });


  $('#stats').html("Showing <span id='numDatapoints'>0</span> datapoints out of <span id='maxDatapoints'>0</span>.<br/>First datapoint added at <span id='firstDatapoint'>Unknown</span>.<br/>Last datapoint added at <span id='lastDatapoint'>Unknown</span>.");
  
  
  $.each(sensors, function(sensorNum, sensorInfo) {
    if ( sensorInfo.type == "web" ) {
      if ( "caller" in sensorInfo ) { 
        var webSensorId = sensorInfo.id;
        var webSensorCaller = sensorInfo.caller;
        var webSensorInterval = sensorInfo.interval;
        sensorInfo.sender = function() {
          genWebMessage(webSensorId, webSensorCaller, function(obj) {
              processMessage(config, obj);
          });
        };
    
        $("#weather").append( $("<span>", {id: "weather_"+webSensorId}) );
        $("#weather").append( $("<br>") );
        
        sensorInfo.sender();
          
        setInterval(function() { sensorInfo.sender(); }, webSensorInterval);
      }
    }
  });
}





function processMessage(config, obj) {
  var sensors = config.sensors;
  var measurements = config.measurements;
  var setup = config.setup;
  // var websensors = setup.websensors;
  var timeData = config.timeData;
  
  console.log("processing message", obj);

  if(!obj.published_at || !("vers" in obj) || !("sens" in obj) ) {
    console.warn("misformed data");
    throw "misformed data";
  }

  //sensorStatus

  for ( var measumentNum = 0; measumentNum < measurements.length; measumentNum++ ) {
    var measurementData = measurements[measumentNum];
    var measurementKey = measurementData.key;
    if (!(measurementKey in obj)) {
      console.warn("misformed data. missing", measurementKey, "key");
      throw "misformed data. missing" + measurementKey + "key";
    }
  }
  
  var sensorIds = {};
  $.each(sensors, function(sensorNum, sensorInfo) {
      sensorIds[sensorInfo.id] = sensorNum;
  });

  var sensorId = obj.device_id;
  var sens = obj.sens;
  var stime = parseTimeStamp(obj.published_at);
  //var vers = obj.vers;

  // console.log("sensorId", sensorId);
  // console.log("sens", sens);
  // console.log("vers", vers);
  // console.log("stime", stime);
  
  if ( !(sensorId in sensorIds) ) {
    console.warn("unknown sensor", sensorId, sensorIds);
    throw "unknown sensor: " + sensorId;
  }

  var input = {};

  var datePos = timeData.length;
  for ( var d=0; d < timeData.length; d++ ) {
    if ( timeData[d].getTime() == stime.getTime() ) { 
        // console.warn("time", stime, "already present");
        datePos = d;
        break;
    }
  }

  timeData[datePos] = stime;

  if ( timeData.length == 1 ) {
    $('#firstDatapoint').html(stime);
  }
  
  $('#numDatapoints').html(timeData.length);
  $('#maxDatapoints').html(setup.maxLen);
  $('#lastDatapoint').html(stime);
  
  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    if ( measurementKey in obj ) {
      input[measurementKey] = obj[measurementKey] === "" ? null : obj[measurementKey];
    } else {
      input[measurementKey] = null;
    }
    // console.log(measurementKey, input[measurementKey]);
  });
 

  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    
    $.each(sensors, function(sensorNum, sensorInfo) {
      var sensorIdL = sensorInfo.id;
      var sensorData = sensorInfo.data;
      //var sensortype = sensorInfo.type;
      var sensorMeasurementData = sensorData[measurementKey].data; 

      if ( sensorId == sensorIdL ) { // correct sensor
          sensorMeasurementData[datePos] = input[measurementKey];
          if ( sens ) {
            if (!( "label_orig" in sensorData[measurementKey] )) {
              sensorData[measurementKey].label_orig  = sensorData[measurementKey].label;
              sensorData[measurementKey].label      += " [" + sens + "]";
            }
          }
      } else { //wrong sensor
        if ( sensorMeasurementData[datePos] ) { // already has data. skip
        } else { // no data. set to null
          if (sensorMeasurementData[datePos -1 ]) {
            sensorMeasurementData[datePos] = sensorMeasurementData[datePos-1];
          } else {
            sensorMeasurementData[datePos] = null;
          }
        }
      }
    });
  });
  
  
  // only keep no more than maxLen points in the line chart  
  if (timeData.length >= setup.maxLen) {
    // console.log('cleaning');
    timeData.shift();
    $.each(measurements, function(measumentNum, measurementData){
      var measurementKey = measurementData.key;
      $.each(sensors, function(sensorNum, sensorInfo) {
        //var sensorIdL = sensorInfo.id;
        sensorInfo.data[measurementKey].data.shift();
      });
    });
  }

  for ( var i=0; i < 2; i++ ) { // run twice to aling lables
    for ( var j=0; j < measurements.length; j++ ) {
      measurements[j].chart.update();
    }
  }
}

function genWebMessage(deviceId, caller, clbk) {
  caller(function(d) {
    
    $("#weather_"+deviceId).html( d.toStr() );
    
    clbk({
      device_id: deviceId,
      sens: "Web",
      vers: "NA",
      published_at: genTimeStamp(),
      temp: d.temperature,
      humi: d.humidity,
      pres: d.pressurePa
    });
  });
}

function genTimeStamp(){
  var a = new Date();
  //"time":"2018:03:10T02:11:57"
  //"published_at":"2018-03-10T15:35:03.526Z",
  var year  = a.getUTCFullYear();
  var month = a.getUTCMonth()+1;
  var date  = a.getUTCDate();
  var hour  = a.getUTCHours();
  var min   = a.getUTCMinutes();
  var sec   = 0; //a.getUTCSeconds();
  var mil   = 0; //a.getUTCMilliseconds();
  var time  = year + '-' + 
  (month < 10 ?                    '0' : '') + month + '-' + 
  (date  < 10 ?                    '0' : '') + date  + 'T' + 
  (hour  < 10 ?                    '0' : '') + hour  + ':' + 
  (min   < 10 ?                    '0' : '') + min   + ':' + 
  (sec   < 10 ?                    '0' : '') + sec   + '.' +
  (mil   < 10 ? '00' : mil < 100 ? '0' : '') + mil   + '.Z';
  return time;
}

function parseTimeStamp(ts) {
  var year  = ts.substr( 0,4);
  var month = ts.substr( 5,2);
  var date  = ts.substr( 8,2);
  var hour  = ts.substr(11,2);
  var min   = ts.substr(14,2);
  var sec   = 0; // ts.substr(17,2);
  var mil   = 0; // ts.substr(20,3);
  var nd    = new Date();
  nd.setUTCFullYear(year);
  nd.setUTCMonth(month-1);
  nd.setUTCDate(date);
  nd.setUTCHours(hour);
  nd.setUTCMinutes(min);
  nd.setUTCSeconds(sec);
  nd.setUTCMilliseconds(mil);
  // console.log(ts+'year' + year + 'month' + month + 'date' + date + 'hour' + hour + 'min' + min + 'sec' + sec, nd);
  return nd;
}

function onSocketError(event) {
  // https://stackoverflow.com/questions/18803971/websocket-onerror-how-to-read-error-description
  var reason;

  // alert("websocket closed", event.code);

  // See http://tools.ietf.org/html/rfc6455#section-7.4.1
  if (event.code == 1000)
      reason = "Normal closure, meaning that the purpose for which the connection was established has been fulfilled.";
  else if(event.code == 1001)
      reason = "An endpoint is \"going away\", such as a server going down or a browser having navigated away from a page.";
  else if(event.code == 1002)
      reason = "An endpoint is terminating the connection due to a protocol error";
  else if(event.code == 1003)
      reason = "An endpoint is terminating the connection because it has received a type of data it cannot accept (e.g., an endpoint that understands only text data MAY send this if it receives a binary message).";
  else if(event.code == 1004)
      reason = "Reserved. The specific meaning might be defined in the future.";
  else if(event.code == 1005)
      reason = "No status code was actually present.";
  else if(event.code == 1006)
     reason = "The connection was closed abnormally, e.g., without sending or receiving a Close control frame";
  else if(event.code == 1007)
      reason = "An endpoint is terminating the connection because it has received data within a message that was not consistent with the type of the message (e.g., non-UTF-8 [http://tools.ietf.org/html/rfc3629] data within a text message).";
  else if(event.code == 1008)
      reason = "An endpoint is terminating the connection because it has received a message that \"violates its policy\". This reason is given either if there is no other sutible reason, or if there is a need to hide specific details about the policy.";
  else if(event.code == 1009)
     reason = "An endpoint is terminating the connection because it has received a message that is too big for it to process.";
  else if(event.code == 1010) // Note that this status code is not used by the server, because it can fail the WebSocket handshake instead.
      reason = "An endpoint (client) is terminating the connection because it has expected the server to negotiate one or more extension, but the server didn't return them in the response message of the WebSocket handshake. <br /> Specifically, the extensions that are needed are: " + event.reason;
  else if(event.code == 1011)
      reason = "A server is terminating the connection because it encountered an unexpected condition that prevented it from fulfilling the request.";
  else if(event.code == 1015)
      reason = "The connection was closed due to a failure to perform a TLS handshake (e.g., the server certificate can't be verified).";
  else
      reason = "Unknown reason";

  // $("body").html("<h1>" + "The connection was closed for reason:</h1><br/><h3>" + reason + "<h3>");
  console.error('The connection was closed for reason: ' + reason);
  displayConnectionInfo("error", "The connection was closed for reason: " + reason);
  window.isConnected = 2;
};

function hexToRGB(hex) {
  //http://www.javascripter.net/faq/hextorgb.htm
  
  var R = hexToR(hex);
  var G = hexToG(hex);
  var B = hexToB(hex);

  // return "rgba(150, 100, 0, 1.0)"
  return [R, G, B];
}

function hexToR(h) { return parseInt((cutHex(h)).substring(0,2),16); }
function hexToG(h) { return parseInt((cutHex(h)).substring(2,4),16); }
function hexToB(h) { return parseInt((cutHex(h)).substring(4,6),16); }
function cutHex(h) { return (h.charAt(0)=="#") ? h.substring(1,7):h; }

function isJson(item) {
  //https://stackoverflow.com/questions/9804777/how-to-test-if-a-string-is-json-or-not
  item = typeof item !== "string" ? JSON.stringify(item) : item;

  try {
    item = JSON.parse(item);
  } catch (e) {
    return [ false, item ];
  }
  
  if (typeof item === "object" && item !== null) {
    return [ true, item ];
  }
  
  return [ false, item ];
}
