$(document).ready(function () {
  var timeData = [];
  
  var sensors = {
    '53ff6d066667574834441267': {
      'name' : 'Neo',
      'color': 'rgba(255, 204, 0, 1)',
      'backgroundColor': 'rgba(255, 204, 0, 0.4)',
      'pos': 0,
      'data': []
    },
    '2e002c000a47343337373738': {
      'name' : 'Morph',
      'color': 'rgba(120, 24, 0, 1)',
      'backgroundColor': 'rgba(120, 24, 0, 0.4)',
      'pos': 1,
      'data': []
    },
    '3d003c000d47343233323032': {
      'name' : 'Trinity',
      'color': 'rgba(24, 120, 0, 1)',
      'backgroundColor': 'rgba(24, 120, 0, 0.4)',
      'pos': 2,
      'data': []
    }
  }
     
  var dataCfg = [];
 
  $.each(sensors, function(sensorName, sensorData) {
    dataCfg[ sensorData['pos'] ] = {
        fill: false,
        label: 'Temperature ' + sensorData['name'],
        yAxisID: 'Temperature',
        borderColor: sensorData['color'],
        pointBoarderColor: sensorData['color'],
        backgroundColor: sensorData['backgroundColor'],
        pointHoverBackgroundColor: sensorData['color'],
        pointHoverBorderColor: sensorData['color'],
        data: sensorData['data']
    };
  });
   
  var data = {
    labels: timeData,
    datasets: dataCfg
  }
  
  var basicOption = {
    title: {
      display: true,
      text: 'Temperature Real-time Data',
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
        $.each(sensors, function(sensorName, sensorData) {
          if ( sensorName == sensorId ) {
            sensorData.data.push(obj.data);
          } else {
            if (sensorData.data.length == 0) {
              sensorData.data.push(null);
            } else {
              sensorData.data.push(sensorData.data[sensorData.data.length-1]);
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
        $.each(sensors, function(sensorName,sensorData) {
          sensorData.data.shift();
        });
      }
          
      myLineChart.update();
    } catch (err) {
      console.error(err);
    }
  }
});

