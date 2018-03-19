/* global palette */
/* global $ */
/* global Chart */

var CONNECT_STATUS_NO           = 0;
var CONNECT_STATUS_CONNECTING   = 1;
var CONNECT_STATUS_RECONNECTING = 2;
var CONNECT_STATUS_CONNECTED    = 3;
var CONNECT_STATUS_UNRESPONSIVE = 4;
var CONNECT_STATUS_DISCONNECTED = 5;


$(document).ready(function () {
  getSensors();
});


function getSensors() {
  $.getJSON("sensors.json", function(config) {
    console.log(config); // this will show the info in firebug console
    
    window.data = config;
    
    loadSensors(config, function(cfg){ loadHistory(cfg, function(c) { startWebSensors(c, function(g) { startWebSocket(g, function(){} ); }); }); });
  })
  .fail(function( jqxhr, textStatus, error ) {
    var err = textStatus + ", " + error;
    console.warn( "Failed to get configuration file: " + err );
    displayConnectionInfo("error", 'Failed to get configuration file: ' + err);
  });
}


function loadSensors(config, clbk) {
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
            
              var Cls = null;
              try {
                Cls = eval(webSensorClassName);
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
            console.warn("websensor ", sensorInfo, "webSensorData not found in setup");
            return;
          }
      }

      
      var rgb = hexToRGB(colorPallet[sensorNum]);
      var mainColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 1.0)";
      var backgroundColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 0.4)";
      
      if ( measumentNum == 0 ) {
        sensorInfo.data = {};
        setSensorLastSeen(sensorInfo, true);
      }
            
      sensorInfo.data[measurementKey] = {
        "fill": fill,
        "type": gtype,
        "yAxisID": axis.id,
        "label": sensorName + (sensorDesc ? " ("+sensorDesc+")" : ""),
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
  // var legendwidth = 0;
  
  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    var measurementLabel = measurementData.label;
    
    //Get the context of the canvas element we want to select
    var $chart = $("<canvas>", { id:"myChart_"+measurementKey }).width(setup.graph.width).height(setup.graph.height);
    
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
    
    
    var legendFontSize = Math.ceil(setup.graph.height / sensors.length); // 60 / 7 = 8.57
    
    var legend = {
      display: true,
      position: setup.graph.labelPosition,
      labels: {
        fontSize: legendFontSize
      }
    };
    
    yaxis.afterFit  = function(scaleInstance) { if (ylabelwidth < scaleInstance.width) { ylabelwidth = scaleInstance.width; } else { scaleInstance.width = ylabelwidth; } };
    xaxis.afterFit  = function(scaleInstance) { if (xlabelwidth < scaleInstance.width) { xlabelwidth = scaleInstance.width; } else { scaleInstance.width = xlabelwidth; } };
    
    measurementData.chart = new Chart(ctx, {
      "type": setup.graph.type,
      "data": {
        labels: config.timeData,
        datasets: measurementData.cfg
      },
      "options": {
        spanGaps: true,
        title: {
          display: setup.graph.displayTitle,
          text: measurementLabel,
          fontSize: setup.graph.titleSize
        },
        scales: {
          yAxes: [yaxis],
          xAxes: [xaxis]
        },
        legend: legend
      }
    });
  });


  $('#stats').html("Showing <span id='numDatapoints'>0</span> datapoints out of <span id='maxDatapoints'>0</span>.<br/>First datapoint added at <span id='firstDatapoint'>Unknown</span>.<br/>Last datapoint added at <span id='lastDatapoint'>Unknown</span>.");

  clbk(config);
}


function loadHistory(config, clbk) {
  var setup = config.setup;
  // "history": {
  //     "display": "lastHour",
  //     "endpoints": { 
  //         "lastHour": "https://hometempfunctionapp.azurewebsites.net/api/oneHourAgo?period=hour",
  //         "lastDay": "https://hometempfunctionapp.azurewebsites.net/api/oneDayAgo?period=day"
  //     }
  // }
  
  var success = false;
  if ( "history" in setup ) {
    console.log("history enabled");
    if ( "display" in setup.history ) {
      console.log("showing history: ", setup.history.display);
      if ( "endpoints" in setup.history ) {
        if ( setup.history.display in setup.history.endpoints) {
          var endpoint = setup.history.endpoints[setup.history.display];
          console.log("endpoint", endpoint);
          success = false; 
          
          success = true; 
          $.getJSON(endpoint, function(history) {
            console.log("history", history);
            config.history = history;
            addHistory(config, clbk);
          })
          .fail(function( jqxhr, textStatus, error ) {
            var err = textStatus + ", " + error;
            console.warn( "Failed to get history data: " + err );
            displayConnectionInfo("error", 'Failed to get history data: ' + err);
            clbk(config);
          });
        }
      }
    }
  }

  if ( ! success ) {
    console.log("history failed. proceeding");
    clbk(config);
  }
}

function addHistory(config, clbk) {
  // {
  //   "period":"hour",
  //   "data": [
  //     {"published_at":"2018-03-17T23:13:10.606Z","_ts":1521328405},
  //     {"published_at":"2018-03-17T22:13:55.64Z","_ts":1521324836}
  //   ]
  // }
  var success = false;
  if ( "history" in config ) {
    if ( "period" in config.history ) {
      var period = config.history.period;
      console.log('adding period: ', period);
      if ( "data" in config.history ) {
        success = true;
        var data = config.history.data;
        var d = 0;
        (function loop() {
            processMessage(config, data[d]);
            if (++d < data.length) {
                setTimeout(loop, 1);  // call myself in 3 seconds time if required
            } else {
              clbk(config);
            }
        })();
      }
    }
  }
  
  if ( ! success ) {
    console.log("failed processing history. proceeding");
    clbk(config);
  }
}

function startWebSensors(config, clbk) {  
  var sensors = config.sensors;

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
  
  clbk(config);
}

function startWebSocket(config, clbk) {
  config.websocket = 'websocket' in config ? config.websocket : {};
  
  config.websocket.isConnected = CONNECT_STATUS_NO;

  connectWebSocket(config);

  setInterval(function() { checkConnection(config); }, 1000);

  clbk(config);
}

function connectWebSocket(config) {
  config.websocket.socket = new WebSocket("wss://" + location.host);
  
  config.websocket.isConnected = CONNECT_STATUS_CONNECTING;
  
  config.websocket.socket.onopen = function () {
    console.log("Successfully connect WebSocket");
    displayConnectionInfo("success", 'Successfully connect WebSocket');
    config.websocket.isConnected = CONNECT_STATUS_CONNECTED;
    config.websocket.connectionTime = new Date().getTime();
    delete config.websocket.lastWebSocketMessage;
  };
  
  config.websocket.socket.onmessage = function (message) {
    console.log("received message" + message.data);
    displayConnectionInfo("good", 'received message');
    
    try {
      var obj = JSON.parse(message.data);
      processMessage(config, obj);
      config.websocket.lastWebSocketMessage = new Date().getTime();
      displayConnectionInfo("good", 'message processed');
      
    } catch (err) {
      console.warn(err);
      displayConnectionInfo("warning", 'error processing message: '+ err);
    }
  };

  config.websocket.socket.onclose = function(ev) { onSocketError(ev, config); };
}

function checkConnection(config) {
  if ( config.websocket.isConnected == CONNECT_STATUS_NO ) { // no status
    // console.log('no status');
    displayConnectionStatus('nostatus');
    return;
  }
  else if ( config.websocket.isConnected == CONNECT_STATUS_CONNECTING ) { // connecting
    // console.log('connecting');
    displayConnectionStatus('connecting');
    return;    
  }
  else if ( config.websocket.isConnected == CONNECT_STATUS_RECONNECTING ) { // reconnecting
    // console.log('connecting');
    displayConnectionStatus('reconnecting');
    return;    
  }
  else if ( config.websocket.isConnected == CONNECT_STATUS_CONNECTED ) { // connected
    // console.log('still connected');
    displayConnectionStatus('connected');
    
    var connectionTime = config.websocket.connectionTime;
    var now = new Date(Date.now()).getTime();
    var diffDate = now - connectionTime;
    if ( "lastWebSocketMessage" in config.websocket ) { // has received messages
      var lastWebSocketMessage = config.websocket.lastWebSocketMessage;
      diffDate = now - lastWebSocketMessage;
    } else { // no messages received
    }

    //console.log("connectionTime", connectionTime, "now", now, "lastWebSocketMessage", config.websocket.lastWebSocketMessage, "diffDate", diffDate, "maxWaitTime", config.websocket.maxWaitTime);

    if ( diffDate > config.websocket.maxWaitTime ) { // set as unresposive
      config.websocket.isConnected = CONNECT_STATUS_UNRESPONSIVE;
    } else { // ok
    }
    
    return;    
  }
  else if ( config.websocket.isConnected == CONNECT_STATUS_UNRESPONSIVE ) { // unresponsive
    config.websocket.isConnected = CONNECT_STATUS_RECONNECTING;
    // console.error('disconnected');
    displayConnectionStatus('unresponsive');
    displayConnectionInfo("warning", "Unresponsive. Disconnecting in 5s");
    setTimeout( function(){ 
      displayConnectionStatus('disconnecting'); 
      displayConnectionInfo("warning", "Unresponsive. Disconnecting");
      try {
        config.websocket.socket.close();
      } catch (e) {}
      delete config.websocket.socket;
      config.websocket.isConnected = CONNECT_STATUS_DISCONNECTED;
    }, 5000);
  }
  else if ( config.websocket.isConnected == CONNECT_STATUS_DISCONNECTED ) { // disconnected
    config.websocket.isConnected = CONNECT_STATUS_CONNECTING;
    // console.error('disconnected');
    displayConnectionStatus('disconnected');
    displayConnectionInfo("warning", "Reconnection in 5s");
    setTimeout( function(){ 
      displayConnectionStatus('connecting'); 
      displayConnectionInfo("warning", "Reconnecting");
      try {
        config.websocket.socket.close();
      } catch (e) {}
      delete config.websocket.socket;
      connectWebSocket(config);
    }, 5000);
  }
}

function onSocketError(event, config) {
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
  config.websocket.isConnected = CONNECT_STATUS_DISCONNECTED;
}

function displayConnectionInfo(level, message) {
  $("#connection").attr('class', level);
  $("#connection").html(message);
}

function displayConnectionStatus(status) {
  $("#connectionStatus").attr('class', status);
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

  
  setSensorLastSeen(sensors[sensorIds[sensorId]]);



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
          sensorMeasurementData[datePos] = null;
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


  var charts = [];
  for ( var j=0; j < measurements.length; j++ ) {
    var chart = measurements[j].chart;
    chart.update();
    charts.push(chart);
  }
  equalizeLegendWidth(charts);
}

function equalizeLegendWidth(charts) {
  // https://codepen.io/anon/pen/LOBPMV
  // https://github.com/chartjs/Chart.js/issues/4982
  var sizes = {height: 0, width: 0, minHeight: 0, minWidth: 0, left: Number.MAX_SAFE_INTEGER};
  
  charts.forEach(function(chart) {
    var legend    = chart.legend;
    var height    = legend.height;
    var width     = legend.width;
    var minHeight = legend.minSize.height;
    var minWidth  = legend.minSize.width;
    var left      = legend.left;
    
    // console.log(legend);
    
    if (sizes.height < height) {
        sizes.height = Math.ceil(height);
    }
    if (sizes.minHeight < minHeight) {
        sizes.minHeight = Math.ceil(minHeight);
    }
    if (sizes.width < width) {
        sizes.width = Math.ceil(width);
    }
    if (sizes.minWidth < minWidth) {
        sizes.minWidth = Math.ceil(minWidth);
    }
    if (sizes.left > left) {
        sizes.left = Math.floor(left);
    }
  });

  //console.log("maxLegendHeight", sizes.height, "maxLegendWidth", sizes.width, "maxLegendLeft", sizes.left, "minHeight", sizes.minHeight, "minWidth", sizes.minWidth);

  charts.forEach(function(chart) {
    chart.legend.height         = sizes.height;
    chart.legend.minSize.height = sizes.minHeight;
    chart.legend.width          = sizes.width;
    chart.legend.minSize.width  = sizes.minWidth;
    chart.legend.left           = sizes.left;
  });
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


function setSensorLastSeen(sensorInfo, isInit) {
  // console.log("setSensorLastSeen", sensorInfo, "isInit", isInit);
  
  var sensorId    = sensorInfo.id;
  var sensorName  = sensorInfo.name;
  var fieldId     = "#sensorStatus_"+sensorId;
  var sensorField = $(fieldId);
  
  if (!( sensorField.length )) {
    $("#sensorStatus").append($("<div>", { id: "sensorStatus_"+sensorId }));
    sensorField = $(fieldId);
  }
  
  var lastSeen = null;
  var seenCount = 0;
  if ( isInit ) {
    sensorInfo.lastSeen = null;
    sensorInfo.seenCount = 0;
    lastSeen = "never";
  } else {
    lastSeen = new Date();
    seenCount = sensorInfo.seenCount + 1;
    // if ( sensorInfo.lastSeen ) { // was seen before
    //   var diffDate = new Date((Date.now()) - sensorInfo.lastSeen);
    //   console.log("diffDate", diffDate);
    //   lastSeen = "Days: " + (diffDate.getDate() - 1) + ", Hours: " + diffDate.getHours() + ", Minutes: " + diffDate.getMinutes() + ", Seconds: " + diffDate.getSeconds() + " ago";
    // } else { // never seen before
    //   if ( ! isInit ) {
    //     lastSeen = "Days: " + (diffDate.getDate() - 1) + ", Hours: " + diffDate.getHours() + ", Minutes: " + diffDate.getMinutes() + ", Seconds: " + diffDate.getSeconds() + " ago";
    //   }
    // }

    sensorInfo.lastSeen = Date.now();
    sensorInfo.seenCount += 1;
  }
    
  $(fieldId).html("last message received from " + sensorName + ": " + lastSeen + ". " + seenCount + " message" + (seenCount != 1 ? "s" : "") + " received so far");
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
