$(document).ready(function () {
  $.getJSON("sensors.json", function(json) {
    console.log(json); // this will show the info it in firebug console
    loadSensors(json);
  })
  .fail(function( jqxhr, textStatus, error ) {
    var err = textStatus + ", " + error;
    console.error( "Request Failed: " + err );
  });
});
  
function loadSensors(config) {
  var timeData = [];     
  var dataCfg = {};
  var sensorIds = {}; 
  var datacfg = {};
  var charts = {};
  var sensors = config.sensors;
  var measurements = config.measurements;
  var setup = config.setup;
  
  
  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    var label = measurementData.label;
    var axis = measurementData.axis;
    var gtype = measurementData.type;
    var fill = measurementData.fill;
    var colorScheme = measurementData.colorScheme;
    var colorPallet = palette(colorScheme, sensors.length);
    dataCfg[measurementKey] = [];

    $.each(sensors, function(sensorNum, sensorInfo) {
      var sensorId = sensorInfo.id;
      var sensorName = sensorInfo.name;
      var rgb = hexToRGB(colorPallet[sensorNum]);
      var mainColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 1.0)";
      var backgroundColor = "rgba("+rgb[0]+", "+rgb[1]+", "+rgb[2]+", 0.4)";
      sensorIds[sensorId] = sensorNum;
      
      if ( measumentNum == 0 ) {
        sensorInfo.data = {};
      }
      
      sensorInfo.data[measurementKey] = {
        "fill": fill,
        "type": gtype,
        "yAxisID": axis.id,
        "label": label + " " + sensorName,
        borderColor: mainColor,
        pointBoarderColor: mainColor,
        pointHoverBorderColor: mainColor,
        pointHoverBackgroundColor: mainColor,
        backgroundColor: backgroundColor,
        data: []
      };
      
      dataCfg[measurementKey].push( sensorInfo.data[measurementKey] );
    });
  });


  $.each(measurements, function(measumentNum, measurementData){
    var measurementKey = measurementData.key;
    var measurementLabel = measurementData.label;
    
    //Get the context of the canvas element we want to select

    var $chart = $("<canvas>", { id:"myChart_"+measurementKey, width:setup.graph.width, height:setup.graph.height});
    
    $( "#graphs" ).append( $chart ); 
    
    // console.log('timeData', timeData);
    // console.log('dataset', dataCfg[measurementKey]);
    // console.log('axis', measurementData.axis);
    
    // var optionsNoAnimation = { animation: false };
    var ctx = document.getElementById("myChart_"+measurementKey).getContext("2d");
    
    charts[measurementKey] = new Chart(ctx, {
      "type": setup.graph.type,
      "data": {
        labels: timeData,
        datasets: dataCfg[measurementKey]
      },
      "options": {
        title: {
          display: setup.graph.displayTitle,
          text: measurementLabel,
          fontSize: setup.graph.titleSize
        },
        scales: {
          yAxes: [measurementData.axis]
        },
        legend: {
          display: true,
          position: setup.graph.labelPosition
        }
      }
    });
  });


  $('#stats').html("Showing <span id='numDatapoints'>0</span> datapoints out of <span id='maxDatapoints'>0</span>.<br/>First datapoint added at <span id='firstDatapoint'>Unknown</span>.<br/>Last datapoint added at <span id='lastDatapoint'>Unknown</span>.");
  
  var ws = new WebSocket("wss://" + location.host);
  ws.onopen = function () {
    console.log("Successfully connect WebSocket");
  }
  
  ws.onmessage = function (message) {
    console.log("receive message" + message.data);

    try {
      var obj = JSON.parse(message.data);
      if(!obj.time || !("vers" in obj) || !("sens" in obj) ) {
        console.log("misformed data")
        return;
      }
      
      for ( var measumentNum = 0; measumentNum < measurements.length; measumentNum++ ) {
        var measurementData = measurements[measumentNum];
        var measurementKey = measurementData.key;
        if (!(measurementKey in obj)) {
          console.log("misformed data. missing", measurementKey, "key");
          return;
        } 
      }

      var sensorId = obj.device_id;
      var sens = obj.sens;
      var vers = obj.vers;
      var stime = obj.time;

      // console.log("sensorId", sensorId);
      // console.log("sens", sens);
      // console.log("vers", vers);
      // console.log("stime", stime);
      
      if ( !(sensorId in sensorIds) ) {
        console.warn("unknown sensor", sensorId, sensorIds);
        return; 
      }
   
      var input = {};

      timeData.push(stime);

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

          if ( sensorId == sensorIdL ) {
              sensorData[measurementKey].data.push(input[measurementKey]);
              if ( sens ) {
                if (!( "label_orig" in sensorData[measurementKey] )) {
                  sensorData[measurementKey].label_orig  = sensorData[measurementKey].label;
                  sensorData[measurementKey].label      += " " + sens;
                  // console.log("new label ", measurementKey, sensorData[measurementKey].label, sensorData[measurementKey].label_orig);
                }
              }
          } else {
            if (sensorData[measurementKey].data.length == 0) {
              sensorData[measurementKey].data.push(null);
            } else {
              sensorData[measurementKey].data.push(sensorData[measurementKey].data[sensorData[measurementKey].data.length-1]);
            }
          }
        });
      });
      
      // // only keep no more than 50 points in the line chart
      
      if (timeData.length > setup.maxLen) {
        // console.log('cleaning');
        timeData.shift();
        $.each(measurements, function(measumentNum, measurementData){
          var measurementKey = measurementData.key;
          $.each(sensors, function(sensorNum, sensorInfo) {
            var sensorIdL = sensorInfo.id;
            var sensorData = sensorInfo.data;
            sensorData[measurementKey].data.shift();
          });
        });
      }
          
      $.each(measurements, function(measumentNum, measurementData){
        var measurementKey = measurementData.key;
        charts[measurementKey].update();
      });
      
    } catch (err) {
      console.error(err);
    }
  }
}

function isJson(item) {
  //https://stackoverflow.com/questions/9804777/how-to-test-if-a-string-is-json-or-not
    item = typeof item !== "string"
        ? JSON.stringify(item)
        : item;

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

function hexToRGB(hex) {
  //http://www.javascripter.net/faq/hextorgb.htm
  
  R = hexToR(hex);
  G = hexToG(hex);
  B = hexToB(hex);

  // return "rgba(150, 100, 0, 1.0)"
  return [R, G, B] 
}

function hexToR(h) {return parseInt((cutHex(h)).substring(0,2),16)}
function hexToG(h) {return parseInt((cutHex(h)).substring(2,4),16)}
function hexToB(h) {return parseInt((cutHex(h)).substring(4,6),16)}
function cutHex(h) {return (h.charAt(0)=="#") ? h.substring(1,7):h}

