//https://www.wunderground.com/weather/api/d/docs

class WunderGround {
	constructor(api_key) {
		//https://api.wunderground.com/api/4e8efbd62eb9ae60/conditions/labels/lang:EN/units:metric/bestfct:1/v:2.0/q/pws:IZAANDIJ6.json?ttl=120
		this.prot    = "https";
		this.api_key = api_key;
		this.lang    = "EN";
		this.units   = "metric";
		this.bestfct = "1";
		this.api_ver = "2.0";
		this.ttl     = "120";
		this.url     = this.prot+"://api.wunderground.com/api/"+this.api_key+"/conditions/labels/lang:"+this.lang+"/units:"+this.units+"/bestfct:"+this.bestfct+"/v:"+this.api_ver+"/q/";//pws:IZAANDIJ6.json?ttl=120";
	}
	
	getById(clbk, city_id) {
		var url = this.url + "pws:" + city_id + ".json";
		this._get(clbk, url);
	}
	
	_get(clbk, url) {
		var q = url + "?ttl=" + this.ttl;
		// console.log("q",q);
		$.getJSON(q, function(json) {
			// console.log(json); // this will show the info it in firebug console
			clbk(new WunderGroundData(json));
		})
		.fail(function( jqxhr, textStatus, error ) {
			var err = textStatus + ", " + error;
			console.error( "Request Failed: " + err );
		});
	}
}


class WunderGroundData {
	constructor(data) {
		this.data = data;
	}

	// "response.version": "2.0",
	get responseVersion() { return this.data.response.version; }
	// "response.units": "metric",
	get responseUnits() { return this.data.response.units; }
	// "response.termsofService": "https://www.wunderground.com/weather/api/d/terms.html",
	get responseTermsofService() { return this.data.response.termsofService; }
	// "response.attribution.image":"//icons.wxug.com/graphics/wu2/logo_130x80.png",
	get responseImage() { return this.data.response.attribution.image; }
	// "response.attribution.title":"Weather Underground",
	get responseTitle() { return this.data.response.attribution.title; }
	// "response.attribution.link":"http://www.wunderground.com"
	get responseLink() { return this.data.response.attribution.link; }
	// "response.features.conditions": 1,
	get responseHasCondition() { return this.data.response.features.conditions == 1; }
	// "response.features.labels": 1
	get responseHasLabels() { return this.data.response.features == 1; }
	// "response.location.name": "Zaandijk",
	get responseName() { return this.data.response.location.name; }
	// "response.location.neighborhood":null,
	get responseNeighborhood() { return this.data.response.location.neighborhood; }
	// "response.location.city": "Zaandijk",
	get responseCity() { return this.data.response.location.city; }
	// "response.location.state": "NH",
	get responseState() { return this.data.response.location.state; }
	// "response.location.state_name":"Netherlands",
	get responseStateName() { return this.data.response.location.state_name; }
	// "response.location.country": "NL",
	get responseCountry() { return this.data.response.location.country; }
	// "response.location.country_iso3166":"NL",
	get responseCountryIso3166() { return this.data.response.location.country_iso3166; }
	// "response.location.country_name":"Netherlands",
	get responseCountryName() { return this.data.response.location.country_name; }
	// "response.location.continent":"EU",
	get responseContinent() { return this.data.response.location.continent; }
	// "response.location.zip":"00000",
	get responseZip() { return this.data.response.location.zip; }
	// "response.location.magic":"63",
	get responseMagic() { return this.data.response.location.magic; }
	// "response.location.wmo":"06257",
	get responseWmo() { return this.data.response.location.wmo; }
	// "response.location.radarcode":"xxx",
	get responseRadarcode() { return this.data.response.location.radarcode; }
	// "response.location.radarregion_ic":null,
	get responseEadarregionIc() { return this.data.response.location.radarregion_ic; }
	// "response.location.radarregion_link": "//",
	get responseRadarregionLink() { return this.data.response.location.radarregion_link; }
	// "response.location.latitude":52.475117,
	get responseLatitude() { return this.data.response.location.latitude; }
	// "response.location.longitude":4.795831,
	get responseLongitude() { return this.data.response.location.longitude; }
	// "response.location.elevation":0.3,
	get responseElevation() { return this.data.response.location.elevation; }
	// "response.location.wfo": null,
	get responseWfo() { return this.data.response.location.wfo; }
	// "response.location.l": "/q/zmw:00000.63.06257",
	get responseL() { return this.data.response.location.l; }
	// "response.location.canonical": "/weather/nl/zaandijk"
	get responseCanonical() { return this.data.response.location.canonical; }
	// "response.date.epoch": 1520771585,
	get responseEpoch() { return this.data.response.date.epoch; }
	// "response.date.pretty": "1:33 PM CET on March 11, 2018",
	get responseDatePretty() { return this.data.response.date.pretty; }
	// "response.date.rfc822": "Sun, 11 Mar 2018 13:33:05 +0100",
	get responseDateRfc822() { return this.data.response.date.rfc822; }
	// "response.date.iso8601": "2018-03-11T13:33:05+0100",
	get responseDateIso8601() { return this.data.response.date.iso8601; }
	// "response.date.year": 2018,
	get responseYear() { return this.data.response.date.year; }
	// "response.date.month": 3,
	get responseMonth() { return this.data.response.date.month; }
	// "response.date.day": 11,
	get responseDay() { return this.data.response.date.day; }
	// "response.date.yday": 69,
	get responseYday() { return this.data.response.date.yday; }
	// "response.date.hour": 13,
	get responseHour() { return this.data.response.date.hour; }
	// "response.date.min": "33",
	get responseMin() { return this.data.response.date.min; }
	// "response.date.sec": 5,
	get responseSec() { return this.data.response.date.sec; }
	// "response.date.monthname": "March",
	get responseMonthName() { return this.data.response.date.monthname; }
	// "response.date.monthname_short": "Mar",
	get responseMonthNameShort() { return this.data.response.date.monthname_short; }
	// "response.date.weekday": "Sunday",
	get responseWeekday() { return this.data.response.date.weekday; }
	// "response.date.weekday_short": "Sun",
	get responseWeekdayShort() { return this.data.response.date.weekday_short; }
	// "response.date.ampm": "PM",
	get responseAmPm() { return this.data.response.date.ampm; }
	// "response.date.tz_short": "CET",
	get responseTzShort() { return this.data.response.date.tz_short; }
	// "response.date.tz_long": "Europe/Amsterdam",
	get responseTzLong() { return this.data.response.date.tz_long; }
	// "response.date.tz_offset_text": "+0100",
	get responseTzOffsetText() { return this.data.response.date.tz_offset_text; }
	// "response.date.tz_offset_hours": 1.00
	get responseTzOffsetHours() { return this.data.response.date.tz_offset_hours; }
	
	// "current_observation.source": "PWS",
	get source() { return this.data.current_observation.source; }
	// "current_observation.station.id":"IZAANDIJ6",
	get id() { return this.data.current_observation.station.id; }
	// "current_observation.station.name":"Zaandijk Rooswijk",
	get name() { return this.data.current_observation.station.name; }
	// "current_observation.station.city":"Zaandijk",
	get city() { return this.data.current_observation.station.city; }
	// "current_observation.station.state":null,
	get state() { return this.data.current_observation.station.state; }
	// "current_observation.station.state_name":"Netherlands",
	get stateName() { return this.data.current_observation.state_name; }
	// "current_observation.station.country":"NL",
	get country() { return this.data.current_observation.station.country; }
	// "current_observation.station.country_name":"Netherlands",
	get countryName() { return this.data.current_observation.station.country_name; }
	// "current_observation.station.country_iso3166":"NL",
	get countryIso3166() { return this.data.current_observation.station.country_iso3166; }
	// "current_observation.station.latitude":52.475117,
	get latitude() { return this.data.current_observation.station.latitude; }
	// "current_observation.station.longitude":4.795831,
	get longitude() { return this.data.current_observation.station.longitude; }
	// "current_observation.station.elevation":0
	get elevation() { return this.data.current_observation.station.elevation; }
	// "current_observation.estimated": null,
	get estimated() { return this.data.current_observation.estimated; }
	
	// "current_observation.date.epoch": 1520771575,
	get dateEpoch() { return this.data.current_observation.date.epoch; }
	// "current_observation.date.pretty": "1:32 PM CET on March 11, 2018",
	get datePretty() { return this.data.current_observation.date.pretty; }
	// "current_observation.date.rfc822": "Sun, 11 Mar 2018 13:32:55 +0100",
	get dateRfc822() { return this.data.current_observation.date.rfc822; }
	// "current_observation.date.iso8601": "2018-03-11T13:32:55+0100",
	get dateIso8601() { return this.data.current_observation.date.iso8601; }
	// "current_observation.date.year": 2018,
	get year() { return this.data.current_observation.date.year; }
	// "current_observation.date.month": 3,
	get month() { return this.data.current_observation.date.month; }
	// "current_observation.date.day": 11,
	get day() { return this.data.current_observation.date.day; }
	// "current_observation.date.yday": 69,
	get yday() { return this.data.current_observation.date.yday; }
	// "current_observation.date.hour": 13,
	get hour() { return this.data.current_observation.date.hour; }
	// "current_observation.date.min": "32",
	get minute() { return this.data.current_observation.date.min; }
	// "current_observation.date.sec": 55,
	get second() { return this.data.current_observation.date.sec; }
	// "current_observation.date.monthname": "March",
	get monthName() { return this.data.current_observation.date.monthname; }
	// "current_observation.date.monthname_short": "Mar",
	get monthNameShort() { return this.data.current_observation.date.monthname_short; }
	// "current_observation.date.weekday": "Sunday",
	get weekDay() { return this.data.current_observation.date.weekday; }
	// "current_observation.date.weekday_short": "Sun",
	get weekDayShort() { return this.data.current_observation.date.weekday_short; }
	// "current_observation.date.ampm": "PM",
	get ampm() { return this.data.current_observation.date.ampm; }
	// "current_observation.date.tz_short": "CET",
	get tzShort() { return this.data.current_observation.date.tz_short; }
	// "current_observation.date.tz_long": "Europe/Amsterdam",
	get tzLong() { return this.data.current_observation.date.tz_long; }
	// "current_observation.date.tz_offset_text": "+0100",
	get tzOffsetText() { return this.data.current_observation.date.tz_offset_text; }
	// "current_observation.date.tz_offset_hours": 1.00
	get tzOffsetHours() { return this.data.current_observation.date.tz_offset_hours; }
	
	// "current_observation.metar": "METAR EHAM 111225Z 22013KT 9999 4500NW FEW002 SCT005 10/09 Q0992 TEMPO 6000 BKN005",
	get metar() { return this.data.current_observation.metar; }
	// "current_observation.condition":"Mostly Cloudy",
	get condition() { return this.data.current_observation.condition; }
	// "current_observation.temperature": 9.1,
	get temperature() { return this.data.current_observation.temperature; }
	// "current_observation.humidity":95,
	get humidity() { return this.data.current_observation.humidity; }
	// "current_observation.wind_speed":1.1,
	get windSpeed() { return this.data.current_observation.wind_speed; }
	// "current_observation.wind_gust_speed":1.8,
	get windGustSpeed() { return this.data.current_observation.wind_gust_speed; }
	// "current_observation.wind_dir":"SSE",
	get windDir() { return this.data.current_observation.wind_dir; }
	// "current_observation.wind_dir_degrees":161,
	get windDirDegrees() { return this.data.current_observation.wind_dir_degrees; }
	// "current_observation.wind_dir_variable":1,
	get windDirVariable() { return this.data.current_observation.wind_dir_variable; }
	// "current_observation.pressure": 992,
	get pressure() { return this.data.current_observation.pressure; }
	// "current_observation.pressure_trend": "+",
	get pressureTrend() { return this.data.current_observation.pressure_trend; }
	// "current_observation.pressure_tendency": 0.000000,
	get pressureTendency() { return this.data.current_observation.pressure_tendency; }
	// "current_observation.pressure_tendency_string": "Steady",
	get pressureTendencyString() { return this.data.current_observation.pressure_tendency_string; }
	// "current_observation.dewpoint": 8,
	get dewpoint() { return this.data.current_observation.dewpoint; }
	// "current_observation.heatindex": null,
	get heatindex() { return this.data.current_observation.heatindex; }
	// "current_observation.windchill": 9,
	get windchill() { return this.data.current_observation.windchill; }
	// "current_observation.feelslike": 9,
	get feelslike() { return this.data.current_observation.feelslike; }
	// "current_observation.visibility":10.0,
	get visibility() { return this.data.current_observation.visibility; }
	// "current_observation.solarradiation": 175,
	get solarRadiation() { return this.data.current_observation.solarradiation; }
	// "current_observation.uv_index": 2.0,
	get uvIndex() { return this.data.current_observation.uv_index; }
	// "current_observation.temperature_indoor": null,
	get temperatureIndoor() { return this.data.current_observation.temperature_indoor; }
	// "current_observation.humidity_indoor": null,
	get humidityIndoor() { return this.data.current_observation.humidity_indoor; }
	// "current_observation.t1f": null,
	get t1f() { return this.data.current_observation.t1f; }
	// "current_observation.t2f": null,
	get t2f() { return this.data.current_observation.t2f; }
	// "current_observation.precip_1hr": 0,
	get precip1hr() { return this.data.current_observation.precip_1hr; }
	// "current_observation.precip_today":4,
	get precipToday() { return this.data.current_observation.precip_today; }
	// "current_observation.soil_temp": null,
	get soilTemp() { return this.data.current_observation.soil_temp; }
	// "current_observation.soil_moisture": null,
	get soilMoisture() { return this.data.current_observation.soil_moisture; }
	// "current_observation.leaf_wetness": null,
	get leafWetness() { return this.data.current_observation.leaf_wetness; }
	// "current_observation.icon":"mostlycloudy",
	get icon() { return this.data.current_observation.icon; }
	// "current_observation.icon_url":"//icons.wxug.com/i/c/v4/mostlycloudy.svg",
	get iconUrl() { return this.data.current_observation.icon_url; }
	// "current_observation.forecast_url":"http://www.wunderground.com/global/stations/06257.html",
	get forecastUrl() { return this.data.current_observation.forecast_url; }
	// "current_observation.history_url":"http://www.wunderground.com/weatherstation/WXDailyHistory.asp?ID=IZAANDIJ6",
	get historyUrl() { return this.data.current_observation.history_url; }
	// "current_observation.ob_url":"http://www.wunderground.com/cgi-bin/findweather/getForecast?query=52.475117,4.795831",
	get obUrl() { return this.data.current_observation.ob_url; }
	// "current_observation.nowcast":"",
	get nowcast() { return this.data.current_observation.nowcast; }
	// "current_observation.pollen": null,
	get pollen() { return this.data.current_observation.pollen; }
	// "current_observation.flu": null,
	get flu() { return this.data.current_observation.flu; }
	// "current_observation.ozone_index": null,
	get ozoneIndex() { return this.data.current_observation.ozone_index; }
	// "current_observation.ozone_text": null,
	get ozoneText() { return this.data.current_observation.ozone_text; }
	// "current_observation.pm_index": null,
	get pmIndex() { return this.data.current_observation.pm_index; }
	// "current_observation.pm_text": null,
	get pmText() { return this.data.current_observation.pm_text; }
	// "current_observation.yesterday_max_temperature": 14.9,
	get yesterdayMaxTemperature() { return this.data.current_observation.yesterday_max_temperature; }
	// "current_observation.yesterday_min_temperature": 6.0,
	get yesterdayMinTemperature() { return this.data.current_observation.yesterday_min_temperature; }
	// "current_observation.yesterday_precip_total": 5.6,
	get yesterdayPrecipTotal() { return this.data.current_observation.yesterday_precip_total; }
	
	
	// "current_observation.cloud_description.highest_layer": "SCT",
	get cloudHighestLayer() { return this.data.current_observation.cloud_description.highest_layer; }
	// "current_observation.cloud_description.oktas": 3,
	get cloudOktas() { return this.data.current_observation.cloud_description.oktas; }
	
	get cloudLayers() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].height": 60,
	get cloudHeight() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].height_char": null,
	get cloudHeightChar() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].cover": "FEW",
	get cloudCover() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].cover_text": "Few",
	get cloudCoverText() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].oktas": "1",
	get cloudOktas() { return this.data.current_observation.cloud_description.layers; }
	// "current_observation.cloud_description.layers[0].other": null
	get cloudOther() { return this.data.current_observation.cloud_description.layers; }
	
	
	// "current_observation.cod_wspd": null,
	get codWspd() { return this.data.current_observation.cod_wspd; }
	// "current_observation.cod_mslp": null,
	get codMslp() { return this.data.current_observation.cod_mslp; }
	// "current_observation.cod_feels_like": null,
	get codFeelsLike() { return this.data.current_observation.cod_feels_like; }
	// "current_observation.cod_vis": null,
	get codVis() { return this.data.current_observation.cod_vis; }
	// "current_observation.cod_altimeter": null,
	get codAltimeter() { return this.data.current_observation.cod_altimeter; }
	// "current_observation.cod_rh": null,
	get codRh() { return this.data.current_observation.cod_rh; }
	// "current_observation.cod_temp_min_24hour": null,
	get codTempMin24hour() { return this.data.current_observation.cod_temp_min_24hour; }
	// "current_observation.cod_snow_season": null,
	get codSnowSeason() { return this.data.current_observation.cod_snow_season; }
	// "current_observation.cod_precip_mtd": null,
	get codPrecipMtd() { return this.data.current_observation.cod_precip_mtd; }
	// "current_observation.cod_snow_6hour": null,
	get codSnow6hour() { return this.data.current_observation.cod_snow_6hour; }
	// "current_observation.cod_temp_max_24hour": null,
	get codTempMax24hour() { return this.data.current_observation.cod_temp_max_24hour; }
	// "current_observation.cod_temp_change_24hour": null,
	get codTempChange24hour() { return this.data.current_observation.cod_temp_change_24hour; }
	// "current_observation.cod_wc": null,
	get codWc() { return this.data.current_observation.cod_wc; }
	// "current_observation.cod_snow_24hour": null,
	get codSnow24hour() { return this.data.current_observation.cod_snow_24hour; }
	// "current_observation.cod_snow_ytd": null,
	get codSnowYtd() { return this.data.current_observation.cod_snow_ytd; }
	// "current_observation.cod_hi": null,
	get codHi() { return this.data.current_observation.cod_hi; }
	// "current_observation.cod_pchange": null,
	get codPchange() { return this.data.current_observation.cod_pchange; }
	// "current_observation.cod_snow_1hour": null,
	get codSnow1hour() { return this.data.current_observation.cod_snow_1hour; }
	// "current_observation.cod_snow_mtd": null,
	get codSnowMtd() { return this.data.current_observation.cod_snow_mtd; }
	// "current_observation.cod_precip_1hour": null,
	get codPrecip1hour() { return this.data.current_observation.cod_precip_1hour; }
	// "current_observation.cod_snow_2day": null,
	get codSnow2day() { return this.data.current_observation.cod_snow_2day; }
	// "current_observation.cod_snow_3day": null,
	get codSnow3day() { return this.data.current_observation.cod_snow_3day; }
	// "current_observation.cod_precip_2day": null,
	get codPrecip2day() { return this.data.current_observation.cod_precip_2day; }
	// "current_observation.cod_snow_7day": null,
	get codSnow7day() { return this.data.current_observation.cod_snow_7day; }
	// "current_observation.cod_precip_3day": null,
	get codPrecip3day() { return this.data.current_observation.cod_precip_3day; }
	// "current_observation.cod_precip_6hour": null,
	get codPrecip6hour() { return this.data.current_observation.cod_precip_6hour; }
	// "current_observation.cod_precip_24hour": null,
	get codPrecip24hour() { return this.data.current_observation.cod_precip_24hour; }
	// "current_observation.cod_precip_ytd": null,
	get codPrecipYtd() { return this.data.current_observation.cod_precip_ytd; }
	// "current_observation.cod_precip_7day": null
	get codPrecip7day() { return this.data.current_observation.cod_precip_7day; }
	
	// "labels.not_available.label": "Not available."
	// "labels.place.label": "Place"
	// "labels.elevation.abbrev": "Elev",
	// "labels.elevation.label": "Elevation",
	// "labels.elevation.units": "m"
	// "labels.temperature.abbrev": "Temp.",
	// "labels.temperature.label": "Temperature",
	// "labels.temperature.units_nosymbol": "C",
	// "labels.temperature.units": "&deg;C"
	// "labels.feelslike.label": "Feels Like",
	// "labels.feelslike.units_nosymbol": "C",
	// "labels.feelslike.units": "&deg;C"
	// "labels.windchill.label": "Wind Chill",
	// "labels.windchill.units_nosymbol": "C",
	// "labels.windchill.units": "&deg;C"
	// "labels.heatindex.label": "Heat Index",
	// "labels.heatindex.units_nosymbol": "C",
	// "labels.heatindex.units": "&deg;C"
	// "labels.dewpoint.label": "Dew Point",
	// "labels.dewpoint.units_nosymbol": "C",
	// "labels.dewpoint.units": "&deg;C"
	// "labels.humidity.label": "Humidity",
	// "labels.humidity.units": "%"
	// "labels.pressure.label": "Pressure",
	// "labels.pressure.units": "hPa"
	// "labels.wind.label": "Wind"
	// "labels.wind_speed.label": "Wind Speed",
	// "labels.wind_speed.separator": "at",
	// "labels.wind_speed.units": "km/h"
	// "labels.wind_direction.label": "Wind Direction",
	// "labels.wind_direction.separator": "from"
	// "labels.wind_gust.label": "Wind Gust",
	// "labels.wind_gust.units": "km/h"
	// "labels.variable.label": "Variable"
	// "labels.calm.label": "Calm"
	// "labels.moisture.label": "Moisture"
	// "labels.rainfall.label": "Rainfall",
	// "labels.rainfall.units": "mm"
	// "labels.snow_depth.label": "Snow Depth",
	// "labels.snow_depth.units": "cm"
	// "labels.visibility.label": "Visibility",
	// "labels.visibility.units": "kilometers"
	// "labels.clouds.label": "Clouds"
	// "labels.health.label": "Health"
	// "labels.air_quality.label": "Air Quality"
	// "labels.flu_tracker.label": "Flu Tracker"
	// "labels.ozone.label": "Ozone"
	// "labels.uv_index.label": "UV Index",
	// "labels.uv_index.abbrev": "UV",
	// "labels.uv_index.separator": "out of"
	// "labels.pollen.label": "Pollen",
	// "labels.pollen.separator": "out of"
	// "labels.time.label": "Time"
	// "labels.now.label": "Now"
	// "labels.today.label": "Today"
	// "labels.tomorrow.label": "Tomorrow"
	// "labels.tomorrownight.label": "Tomorrow Night"
	// "labels.sunday.abbrev": "Sun",
	// "labels.sunday.label": "Sunday"
	// "labels.monday.abbrev": "Mon",
	// "labels.monday.label": "Monday"
	// "labels.tuesday.abbrev": "Tue",
	// "labels.tuesday.label": "Tuesday"
	// "labels.wednesday.abbrev": "Wed",
	// "labels.wednesday.label": "Wednesday"
	// "labels.thursday.abbrev": "Thu",
	// "labels.thursday.label": "Thursday"
	// "labels.friday.abbrev": "Fri",
	// "labels.friday.label": "Friday"
	// "labels.saturday.abbrev": "Sat",
	// "labels.saturday.label": "Saturday"
	// "labels.sundaynight.label": "Sunday Night"
	// "labels.mondaynight.label": "Monday Night"
	// "labels.tuesdaynight.label": "Tuesday Night"
	// "labels.wednesdaynight.label": "Wednesday Night"
	// "labels.thursdaynight.label": "Thursday Night"
	// "labels.fridaynight.label": "Friday Night"
	// "labels.saturdaynight.label": "Saturday Night"
	// "labels.sunrise.label": "Sunrise"
	// "labels.sunset.label": "Sunset"
	// "labels.moon.label": "Moon"
	// "labels.nomoonrise.label": "No Moon Rise"
	// "labels.nomoonset.label": "No Moon Set"
	// "labels.weatherstation.label": "Weather Station"
	// "labels.no_reporting.label": "No Stations Reporting"
	// "labels.pws.label": "Personal Weather Station"
	// "labels.airport.label": "Airport"
	// "labels.updated.label": "Updated"
	// "labels.source.label": "Source"
	// "labels.pop.abbrev": "Chance of Precip.",
	// "labels.pop.label": "chance of precipitation",
	// "labels.pop.units": "%"
	// "labels.chancerain.label": "Chance of Rain",
	// "labels.chancerain.units": "%"
	// "labels.chancesnow.label": "Chance of Snow",
	// "labels.chancesnow.units": "%"
	// "labels.precipitation.abbrev": "Precip.",
	// "labels.precipitation.label": "Precipitation"
	// "labels.cloudcover.label": "Cloud Cover",
	// "labels.cloudcover.units": "%"
	// "labels.conditions.label": "Conditions"
	// "labels.current_conditions.label": "Current Conditions"
	// "labels.north.abbrev": "N",
	// "labels.north.label": "North"
	// "labels.east.abbrev": "E",
	// "labels.east.label": "East"
	// "labels.south.abbrev": "S",
	// "labels.south.label": "South"
	// "labels.west.abbrev": "W",
	// "labels.west.label": "West"
	// "labels.distance.label": "Distance",
	// "labels.distance.units": "km"

	get coordData() { return [this.coordLat, this.coordLon]; }
	get coordLat()  { return this.latitude; }
	get coordLon()  { return this.longitude; }
	
	get weatherData()        { return [this.weatherClass, this.weatherDescription]; }
	get weatherClass()       { return null; }
	get weatherDescription() { return null; }
	
	get temperatureMin() { return null; }
	get temperatureMax() { return null; }
	get _pressure()      { return this.pressure; }
	get pressurePa()     { return this._pressure * 100; }
	get pressureHPa()    { return this._pressure; }
	
	get windData()   { return [this.windSpeed, this.windDeg]; }
	get windDeg()    { return this.windDirDegrees; }
	
	get clouds()     { return this.condition; }
	
	get time()       { return new Date(this.dateEpoch*1000); }
	
	get sunrise()     { return null; }
	get sunset()      { return null; }
	get code()        { return "error" in this.data ? 404 : 200; }
	
	get isValid()     { return this.code == 200; }
	
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



//https://www.wunderground.com/weather/api/d/docs
//http://api.wunderground.com/api/4e8efbd62eb9ae60/conditions/q/CA/San_Francisco.json
//http://api.wunderground.com/api/4e8efbd62eb9ae60/geolookup/conditions/forecast/q/Australia/Sydney.json
//http://api.wunderground.com/api/4e8efbd62eb9ae60/geolookup/q/France/Paris.json

//https://api-ak.wunderground.com/api/d8585d80376a429e/conditions/labels/lang:EN/units:english/bestfct:1/v:2.0/q/pws:IZAANDIJ6.json?ttl=120

// {
// 	"response": {
// 		"version": "2.0",
// 		"units": "metric",
// 		"termsofService": "https://www.wunderground.com/weather/api/d/terms.html",
// 		"attribution": {
// 			"image":"//icons.wxug.com/graphics/wu2/logo_130x80.png",
// 			"title":"Weather Underground",
// 			"link":"http://www.wunderground.com"
// 		},
// 		"features": {
// 			"conditions": 1,
// 			"labels": 1
// 		},
// 		"location": {
// 			"name": "Zaandijk",
// 			"neighborhood":null,
// 			"city": "Zaandijk",
// 			"state": "NH",
// 			"state_name":"Netherlands",
// 			"country": "NL",
// 			"country_iso3166":"NL",
// 			"country_name":"Netherlands",
// 			"continent":"EU",
// 			"zip":"00000",
// 			"magic":"63",
// 			"wmo":"06257",
// 			"radarcode":"xxx",
// 			"radarregion_ic":null,
// 			"radarregion_link": "//",
// 			"latitude":52.475117,
// 			"longitude":4.795831,
// 			"elevation":0.3,
// 			"wfo": null,
// 			"l": "/q/zmw:00000.63.06257",
// 			"canonical": "/weather/nl/zaandijk"
// 		},
// 		"date": {
// 			"epoch": 1520771585,
// 			"pretty": "1:33 PM CET on March 11, 2018",
// 			"rfc822": "Sun, 11 Mar 2018 13:33:05 +0100",
// 			"iso8601": "2018-03-11T13:33:05+0100",
// 			"year": 2018,
// 			"month": 3,
// 			"day": 11,
// 			"yday": 69,
// 			"hour": 13,
// 			"min": "33",
// 			"sec": 5,
// 			"monthname": "March",
// 			"monthname_short": "Mar",
// 			"weekday": "Sunday",
// 			"weekday_short": "Sun",
// 			"ampm": "PM",
// 			"tz_short": "CET",
// 			"tz_long": "Europe/Amsterdam",
// 			"tz_offset_text": "+0100",
// 			"tz_offset_hours": 1.00
// 		}
// 	},
// 	"current_observation": {
// 		"source": "PWS",
// 		"station": {
// 			"id":"IZAANDIJ6",
// 			"name":"Zaandijk Rooswijk",
// 			"city":"Zaandijk",
// 			"state":null,
// 			"state_name":"Netherlands",
// 			"country":"NL",
// 			"country_name":"Netherlands",
// 			"country_iso3166":"NL",
// 			"latitude":52.475117,
// 			"longitude":4.795831,
// 			"elevation":0
// 		},
// 		"estimated": null,
// 		"date": {
// 			"epoch": 1520771575,
// 			"pretty": "1:32 PM CET on March 11, 2018",
// 			"rfc822": "Sun, 11 Mar 2018 13:32:55 +0100",
// 			"iso8601": "2018-03-11T13:32:55+0100",
// 			"year": 2018,
// 			"month": 3,
// 			"day": 11,
// 			"yday": 69,
// 			"hour": 13,
// 			"min": "32",
// 			"sec": 55,
// 			"monthname": "March",
// 			"monthname_short": "Mar",
// 			"weekday": "Sunday",
// 			"weekday_short": "Sun",
// 			"ampm": "PM",
// 			"tz_short": "CET",
// 			"tz_long": "Europe/Amsterdam",
// 			"tz_offset_text": "+0100",
// 			"tz_offset_hours": 1.00
// 		},
// 		"metar": "METAR EHAM 111225Z 22013KT 9999 4500NW FEW002 SCT005 10/09 Q0992 TEMPO 6000 BKN005",
// 		"condition":"Mostly Cloudy",
// 		"temperature": 9.1,
// 		"humidity":95,
// 		"wind_speed":1.1,
// 		"wind_gust_speed":1.8,
// 		"wind_dir":"SSE",
// 		"wind_dir_degrees":161,
// 		"wind_dir_variable":1,
// 		"pressure": 992,
// 		"pressure_trend": "+",
// 		"pressure_tendency": 0.000000,
// 		"pressure_tendency_string": "Steady",
// 		"dewpoint": 8,
// 		"heatindex": null,
// 		"windchill": 9,
// 		"feelslike": 9,
// 		"visibility":10.0,
// 		"cloud_description": {
// 			"highest_layer": "SCT",
// 			"oktas": 3,
// 			"layers": [
// 				{
//	 				"height": 60,
// 					"height_char": null,
// 					"cover": "FEW",
// 					"cover_text": "Few",
// 					"oktas": "1",
//	 				"other": null
// 				},
// 				{
// 					"height": 152,
// 					"height_char": null,
// 					"cover": "SCT",
// 					"cover_text": "Scattered Clouds",
//	 				"oktas": "3",
// 					"other": null
// 				}
// 			]
// 		},
// 		"solarradiation": 175,
// 		"uv_index": 2.0,
// 		"temperature_indoor": null,
// 		"humidity_indoor": null,
// 		"t1f": null,
// 		"t2f": null,
// 		"precip_1hr": 0,
// 		"precip_today":4,
// 		"soil_temp": null,
// 		"soil_moisture": null,
// 		"leaf_wetness": null,
// 		"icon":"mostlycloudy",
// 		"icon_url":"//icons.wxug.com/i/c/v4/mostlycloudy.svg",
// 		"forecast_url":"http://www.wunderground.com/global/stations/06257.html",
// 		"history_url":"http://www.wunderground.com/weatherstation/WXDailyHistory.asp?ID=IZAANDIJ6",
// 		"ob_url":"http://www.wunderground.com/cgi-bin/findweather/getForecast?query=52.475117,4.795831",
// 		"nowcast":"",
// 		"pollen": null,
// 		"flu": null,
// 		"ozone_index": null,
// 		"ozone_text": null,
// 		"pm_index": null,
// 		"pm_text": null,
// 		"yesterday_max_temperature": 14.9,
// 		"yesterday_min_temperature": 6.0,
// 		"yesterday_precip_total": 5.6,
// 		"cod_wspd": null,
// 		"cod_mslp": null,
// 		"cod_feels_like": null,
// 		"cod_vis": null,
// 		"cod_altimeter": null,
// 		"cod_rh": null,
// 		"cod_temp_min_24hour": null,
// 		"cod_snow_season": null,
// 		"cod_precip_mtd": null,
// 		"cod_snow_6hour": null,
// 		"cod_temp_max_24hour": null,
// 		"cod_temp_change_24hour": null,
// 		"cod_wc": null,
// 		"cod_snow_24hour": null,
// 		"cod_snow_ytd": null,
// 		"cod_hi": null,
// 		"cod_pchange": null,
// 		"cod_snow_1hour": null,
// 		"cod_snow_mtd": null,
// 		"cod_precip_1hour": null,
// 		"cod_snow_2day": null,
// 		"cod_snow_3day": null,
// 		"cod_precip_2day": null,
// 		"cod_snow_7day": null,
// 		"cod_precip_3day": null,
// 		"cod_precip_6hour": null,
// 		"cod_precip_24hour": null,
// 		"cod_precip_ytd": null,
// 		"cod_precip_7day": null
// 	},
// 	"labels": {
// 		"not_available": {
// 			 "label": "Not available."
// 		},
// 		"place": {
// 			"label": "Place"
// 		},
// 		"elevation": {
// 			"abbrev": "Elev",
// 		  	"label": "Elevation",
// 		  	"units": "m"
// 		},
// 		"temperature": {
// 			"abbrev": "Temp.",
// 			"label": "Temperature",
// 			"units_nosymbol": "C",
// 			"units": "&deg;C"
// 		},
// 		"feelslike": {
// 			"label": "Feels Like",
// 			"units_nosymbol": "C",
// 			"units": "&deg;C"
// 		},
// 		"windchill": {
// 			"label": "Wind Chill",
// 			"label": "Temperature",
// 			"units_nosymbol": "C",
// 			"units": "&deg;C"
// 		},
// 		"heatindex": {
// 			"label": "Heat Index",
// 			"label": "Temperature",
// 			"units_nosymbol": "C",
// 			"units": "&deg;C"
// 		},
// 		"dewpoint": {
// 			"label": "Dew Point",
// 			"units_nosymbol": "C",
// 			"units": "&deg;C"
// 		},
// 		"humidity": {
// 			"label": "Humidity",
// 			"units": "%"
// 		},
// 		"pressure": {
// 			"label": "Pressure",
// 			"units": "hPa"
// 		},
// 		"wind": {
// 			"label": "Wind"
// 		},
// 		"wind_speed": {
// 			"label": "Wind Speed",
// 			"separator": "at",
// 			"units": "km/h"
// 		},
// 		"wind_direction": {
// 			"label": "Wind Direction",
// 			"separator": "from"
// 		},
// 		"wind_gust": {
// 			"label": "Wind Gust",
// 			"units": "km/h"
// 		},
// 		"variable": {
// 			"label": "Variable"
// 		},
// 		"calm": {
// 			"label": "Calm"
// 		},
// 		"moisture": {
// 			"label": "Moisture"
// 		},
// 		"rainfall": {
// 			"label": "Rainfall",
// 			"units": "mm"
// 		},
// 		"snow_depth": {
// 			"label": "Snow Depth",
// 			"units": "cm"
// 		},
// 		"visibility": {
// 			"label": "Visibility",
// 			"units": "kilometers"
// 		},
// 		"clouds": {
// 			"label": "Clouds"
// 		},
// 		"health": {
// 			"label": "Health"
// 		},
// 		"air_quality": {
// 			"label": "Air Quality"
// 		},
// 		"flu_tracker": {
// 			"label": "Flu Tracker"
// 		},
// 		"ozone": {
// 			"label": "Ozone"
// 		},
// 		"uv_index": {
// 			"label": "UV Index",
// 			"abbrev": "UV",
// 			"separator": "out of"
// 		},
// 		"pollen": {
// 			"label": "Pollen",
// 			"separator": "out of"
// 		},
// 		"time": {
// 			"label": "Time"
// 		},
// 		"now": {
// 			"label": "Now"
// 		},
// 		"today": {
// 			"label": "Today"
// 		},
// 		"tomorrow": {
// 			"label": "Tomorrow"
// 		},
// 		"tomorrownight": {
// 			"label": "Tomorrow Night"
// 		},
// 		"sunday": {
// 			"abbrev": "Sun",
// 			"label": "Sunday"
// 		},
// 		"monday": {
// 			"abbrev": "Mon",
// 			"label": "Monday"
// 		},
// 		"tuesday": {
// 			"abbrev": "Tue",
// 			"label": "Tuesday"
// 		},
// 		"wednesday": {
// 			"abbrev": "Wed",
// 			"label": "Wednesday"
// 		},
// 		"thursday": {
// 			"abbrev": "Thu",
// 			"label": "Thursday"
// 		},
// 		"friday": {
// 			"abbrev": "Fri",
// 			"label": "Friday"
// 		},
// 		"saturday": {
// 			"abbrev": "Sat",
// 			"label": "Saturday"
// 		},
// 		"sundaynight": {
// 			"label": "Sunday Night"
// 		},
// 		"mondaynight": {
// 			"label": "Monday Night"
// 		},
// 		"tuesdaynight": {
// 			"label": "Tuesday Night"
// 		},
// 		"wednesdaynight": {
// 			"label": "Wednesday Night"
// 		},
// 		"thursdaynight": {
// 			"label": "Thursday Night"
// 		},
// 		"fridaynight": {
// 			"label": "Friday Night"
// 		},
// 		"saturdaynight": {
// 			"label": "Saturday Night"
// 		},
// 		"sunrise": {
// 			"label": "Sunrise"
// 		},
// 		"sunset": {
// 			"label": "Sunset"
// 		},
// 		"moon": {
// 			"label": "Moon"
// 		},
// 		"nomoonrise": {
// 			"label": "No Moon Rise"
// 		},
// 		"nomoonset": {
// 			"label": "No Moon Set"
// 		},
// 		"weatherstation": {
// 			"label": "Weather Station"
// 		},
// 		"no_reporting": {
// 			"label": "No Stations Reporting"
// 		},
// 		"pws": {
// 			"label": "Personal Weather Station"
// 		},
// 		"airport": {
// 			"label": "Airport"
// 		},
// 		"updated": {
// 			"label": "Updated"
// 		},
// 		"source": {
// 			"label": "Source"
// 		},
// 		"pop": {
// 			"abbrev": "Chance of Precip.",
// 			"label": "chance of precipitation",
// 			"units": "%"
// 		},
// 		"chancerain": {
// 			"label": "Chance of Rain",
// 			"units": "%"
// 		},
// 		"chancesnow": {
// 			"label": "Chance of Snow",
// 			"units": "%"
// 		},
// 		"precipitation": {
// 			"abbrev": "Precip.",
// 			"label": "Precipitation"
// 		},
// 		"cloudcover": {
// 			"label": "Cloud Cover",
// 			"units": "%"
// 		},
// 		"conditions": {
// 			"label": "Conditions"
// 		},
// 		"current_conditions": {
// 			"label": "Current Conditions"
// 		},
// 		"north": {
// 			"abbrev": "N",
// 			"label": "North"
// 		},
// 		"east": {
// 			"abbrev": "E",
// 			"label": "East"
// 		},
// 		"south": {
// 			"abbrev": "S",
// 			"label": "South"
// 		},
// 		"west": {
// 			"abbrev": "W",
// 			"label": "West"
// 		},
// 		"distance": {
// 			"label": "Distance",
// 			"units": "km"
// 		}
// 	}
// }


// image.url
// image.title
// image.link
// display_location.full
// display_location.city
// display_location.state
// display_location.state_name
// display_location.country
// display_location.country_iso3166
// display_location.zip
// display_location.latitude
// display_location.longitude
// display_location.elevation
// observation_location.full
// observation_location.city
// observation_location.state
// observation_location.country
// observation_location.country_iso3166
// observation_location.latitude
// observation_location.longitude
// observation_location.elevation
// estimated
// station_id
// observation_time
// observation_time_rfc822
// observation_epoch
// local_time_rfc822
// local_epoch
// local_tz_short
// local_tz_long
// local_tz_offset
// weather
// temperature_string
// temp_f
// temp_c
// relative_humidity
// wind_string
// wind_dir
// wind_degrees
// wind_mph
// wind_gust_mph
// wind_kph
// wind_gust_kph
// pressure_mb
// pressure_in
// pressure_trend
// dewpoint_string
// dewpoint_f
// dewpoint_c
// heat_index_string
// heat_index_f
// heat_index_c
// windchill_string
// windchill_f
// windchill_c
// feelslike_string
// feelslike_f
// feelslike_c
// visibility_mi
// visibility_km
// solarradiation
// UV
// precip_1hr_string
// precip_1hr_in
// precip_1hr_metric
// precip_today_string
// precip_today_in
// precip_today_metric
// icon
// icon_url
// forecast_url
// history_url
// ob_url
