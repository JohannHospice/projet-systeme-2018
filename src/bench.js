'use strict'

var child = require('child_process'), fs = require('fs');
const SEPARATOR = ";"

const config = {
	iterations: 5,
	threadsMin: 1,
	threadsMax: 10,
	dataSizeMin: 1000000,
	dataSizeInc: 1000000,
	dataSizeMax: 10000000,
};

(function(){
	function executionTime(command, n, t){
		let comOpt = command + " -t " + t + " -n " + n;
		console.log(comOpt)
		return parseFloat(child.execSync(comOpt, {encoding: "utf8"})
			.match(/Done in ?(\d*\.\d*) seconds\./i)[1])
	}

	function executionTimeAvg(command, n, t){
		let timesum = 0
		for (var i = 0; i < config.iterations; i++)
			timesum += executionTime(command, n, t)
		return timesum / config.iterations
	}

	function executionTimeArr(command){
		var arr = []
		for (var n = config.dataSizeMin; n < config.dataSizeMax; n += config.dataSizeInc) {
			let arr1 = [n]
			for (var t = config.threadsMin; t < config.threadsMax; t++) {
				arr1.push(executionTimeAvg(command, n, t))
			}
			arr.push(arr1)
		}
		return arr
	}

	function zort(command){
		var arr = executionTimeArr(command)
		return arr.map(line => line.map(v => v.toString().replace(/\./gi, ',').slice(0, 9)).join(SEPARATOR)).join("\n")
	}

	function bench(command) {
		var title = command
		console.log(command)
		for (var t = config.threadsMin; t < config.threadsMax; t++) 
			title += SEPARATOR + t
		var z = zort(command)
		return title + "\n" + z
	}

	var result = [bench("./stealing"), bench("./sharing")].join("\n")

	fs.writeFile("./bench.csv", result, function(err) {
	    if(err)
	        return console.log(err);
	    console.log("The file was saved!");
	}); 

})()
