$(document).ready(function () {
  $.getJSON("sensors.json", function(json) {
    console.log(json); // this will show the info it in firebug console
    loadSensors(json);
  });
});
  
function loadSensors(sensors) {
  var timeData = [];     
  var dataCfg = [];
 
  $.each(sensors, function(sensorName, sensorData) {
    sensorData.meta = {
      'temp': {
        fill: false,
        type: 'line',
        yAxisID: 'Temperature',
        label: 'Temperature ' + sensorData.name,
        borderColor: sensorData.color.temp,
        pointBoarderColor: sensorData.color.temp,
        backgroundColor: sensorData.backgroundColor.temp,
        pointHoverBackgroundColor: sensorData.color.temp,
        pointHoverBorderColor: sensorData.color.temp,
        data: sensorData.data.temp
      },
      'humi': {
        fill: false,
        type: 'bar',
        yAxisID: 'Humidity',
        label: 'Humidity ' + sensorData.name,
        borderColor: sensorData.color.humi,
        pointBoarderColor: sensorData.color.humi,
        backgroundColor: sensorData.backgroundColor.humi,
        pointHoverBackgroundColor: sensorData.color.humi,
        pointHoverBorderColor: sensorData.color.humi,
        data: sensorData.data.humi
      }
    };
    
    dataCfg[ sensorData.pos * 2 + 0 ] = sensorData.meta.temp;
    dataCfg[ sensorData.pos * 2 + 1 ] = sensorData.meta.humi;
  });
      
  var data = {
    labels: timeData,
    datasets: dataCfg
  }
  
  var basicOption = {
    title: {
      display: true,
      text: 'Temperature & Humidity Real-time Data',
      fontSize: 36
    },
    scales: {
      yAxes: [{
        id: 'Temperature',
        type: 'linear',
        scaleLabel: {
          labelString: 'Temperature(C)',
          display: true
        },
        position: 'left',
      },
      {
        id: 'Humidity',
        type: 'linear',
        scaleLabel: {
          labelString: 'Humidity(%)',
          display: true
        },
        position: 'right',
      }]
    }
  }
  
  //Get the context of the canvas element we want to select
  var ctx = document.getElementById("myChart").getContext("2d");
  var optionsNoAnimation = { animation: false }
  var myLineChart = new Chart(ctx, {
    'type': 'line',
    'data': data,
    'options': basicOption
  });

  const maxLen = 50;

  var ws = new WebSocket('wss://' + location.host);
  ws.onopen = function () {
    console.log('Successfully connect WebSocket');
  }
  ws.onmessage = function (message) {
    console.log('receive message' + message.data);

    try {
      var obj = JSON.parse(message.data);
      if(!obj.time || !obj.data) {
        console.log('misformed data')
        return;
      }
      
      timeData.push(obj.time);

      var sensorId = obj.device_id;
      
      if ( sensorId in sensors) {
        var jr = isJson(obj.data)
        var isjson = jr[0];
        var parsed = jr[1];
        console.log('isjson', isjson, 'parsed', parsed);
        var temp = null
        var humi = null;
        var sensor = null;
        
        if ( isjson ) {
          if ( 'temp' in parsed ) {
            temp = parsed.temp;
          }
          if ( 'humi' in parsed ) {
            humi = parsed.humi;
          }
          if ( 'sensor' in parsed ) {
            sensor = parsed.sensor;
          }           
        } else {
            temp = parsed;          
        }
        
        $.each(sensors, function(sensorName, sensorData) {
          if ( sensorName == sensorId ) {
              sensorData.data.temp.push(temp);
              sensorData.data.humi.push(humi);
              if (sensor) {
                if (!( 'label_orig' in sensorData.meta.temp )) {
                  sensorData.meta.temp.label_orig  = sensorData.meta.temp.label;
                  sensorData.meta.humi.label_orig  = sensorData.meta.humi.label;
                  sensorData.meta.temp.label      += ' ' + sensor;
                  sensorData.meta.humi.label      += ' ' + sensor;
                  console.log('new label temp', sensorData.meta.temp.label, sensorData.meta.temp.label_orig);
                  console.log('new label humi', sensorData.meta.humi.label, sensorData.meta.humi.label_orig); 
                }
              }
          } else {
            if (sensorData.data.length == 0) {
              sensorData.data.temp.push(null);
              sensorData.data.humi.push(null);
            } else {
              sensorData.data.temp.push(sensorData.data.temp[sensorData.data.temp.length-1]);
              sensorData.data.humi.push(sensorData.data.humi[sensorData.data.humi.length-1]);
            }
          }
        });
      } else {
        console.warn('unknown sensor', sensorId);
      }
      
      // // only keep no more than 50 points in the line chart
      
      var len = timeData.length;
      if (len > maxLen) {
        timeData.shift();
        $.each(sensors, function(sensorName, sensorData) {
          sensorData.data.temp.shift();
          sensorData.data.humi.shift();
        });
      }
          
      myLineChart.update();
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
