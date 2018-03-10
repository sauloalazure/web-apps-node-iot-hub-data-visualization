//https://openweathermap.org/current

class OpenWeatherData {
	// {
	// "coord": {"lon":4.89,"lat":52.37},
	// "weather":[
	// 		{"id":801,"main":"Clouds","description":"few clouds","icon":"02d"}
	// 	],
	// "base":"stations",
	// "main":{"temp":14,"pressure":998,"humidity":76,"temp_min":14,"temp_max":14},
	// "visibility":10000,
	// "wind":{"speed":6.7,"deg":200},
	// "clouds":{"all":20},
	// "dt":1520682900,
	// "sys":{"type":1,"id":5204,"message":0.0037,"country":"NL","sunrise":1520661929,"sunset":1520703397},
	// "id":2759794,
	// "name":"Amsterdam",
	// "cod":200
	// }
	constructor(data) {
		this.data = data;
	}
	
	get coordData() { return [this.coordLat, this.coordLon]; }
	get coordLat()  { return this.data.coord.lat; }
	get coordLon()  { return this.data.coord.lat; }
	
	get weatherData()        { return [this.weatherClass, this.weatherDescription]; }
	get weatherClass()       { return this.data.weather[0].main; }
	get weatherDescription() { return this.data.weather[0].description; }
	
	get temperature()    { return this.data.main.temp; }
	get temperatureMin() { return this.data.main.temp_min; }
	get temperatureMax() { return this.data.main.temp_max; }
	get humidity()       { return this.data.main.humidity; }
	get _pressure()      { return this.data.main.pressure; }
	get pressurePa()     { return this._pressure * 100; }
	get pressureHPa()    { return this._pressure; }
	
	get visibility() { return this.data.visibility; }
	
	get windData()   { return [this.windSpeed, this.windDeg]; }
	get windSpeed()  { return this.data.wind.speed; }
	get windDeg()    { return this.data.wind.deg; }
	
	get clouds()     { return this.data.clouds.all; }
	
	get time()       { return new Date(this.data.dt*1000); }
	
	get sunrise()     { return new Date(this.data.sys.sunrise*1000); }
	get sunset()      { return new Date(this.data.sys.sunset*1000); }
	get country()     { return this.data.sys.country; }
	get id()          { return this.data.sys.id; }
	
	get name()        { return this.data.name; }
	get code()        { return this.data.cod; }
	
	get isValid()     { return this.data.cod == 200; }
	
	toStr() {
		if ( ! this.isValid ) {
			return "No valid data received";
		} else {
			return "The current weather in " + this.name + " " + 
			"[" + this.country + ", " + this.id + "] is a " + 
			(this.temperature < 15 ? "cold" : "warm") + " " +
			this.temperature + " degrees (C) " +
			"with wind speed of " + this.windSpeed + "m/s" +
			", " + this.clouds + "% cloud coverage" +
			", air pressure of " + this.pressurePa + "Pa" + ", " + 
			"being classified as a " + this.weatherDescription + " day.<br/>" +
			"You should expect sunrise at " + this.sunrise + " " +
			"and sunset at " + this.sunset + ".<br/>" +
			"This information was last updated at " + this.time + "<br/>" 
			;
		}
	}
}

class OpenWeather {
	constructor(api_key) {
		this.api_key = api_key;
		this.api_ver = "2.5";
		this.prot = "https";
		this.units = "metric";
		this.url = this.prot+"://api.openweathermap.org/data/"+this.api_ver;
	}
	
	getByName(clbk, city_name) {
		var url = this.url + "/weather?q=" + city_name;
		this._get(clbk, url);
	}
	
	getByLocation(clbk, lat, lon) {
		var url = this.url + "/weather?lat=" + lat + "&lon=" + lon;
		this._get(clbk, url);
	}
	
	getByZip(clbk, zip) {
		var url = this.url + "/weather?zip=" + zip;
		this._get(clbk, url);
	}
	
	getById(clbk, city_id) {
		var url = this.url + "/weather?id=" + city_id;
		this._get(clbk, url);
	}
	
	_get(clbk, url) {
		var q = url + "&units=" + this.units + "&appid=" + this.api_key;
		// console.log("q",q);
		$.getJSON(q, function(json) {
			// console.log(json); // this will show the info it in firebug console
			clbk(new OpenWeatherData(json));
		})
		.fail(function( jqxhr, textStatus, error ) {
			var err = textStatus + ", " + error;
			console.error( "Request Failed: " + err );
		});
	}
}
