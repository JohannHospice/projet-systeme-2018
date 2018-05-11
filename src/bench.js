'use strict'

var child = require('child_process');
var fs = require('fs');
const SEPARATOR = ";", TSTART = 1, NSTART = 0;
const N = 25, T = 16, it = 10

function executionTime(command, n, t){
	return parseFloat(child.execSync(command + " -t " + t + " -n " + n, {encoding: "utf8"})
		.match(/Done in ?(\d\.\d*) seconds\./i)[1])
}

function executionTimeAvg(command, n, t, it){
	let timesum = 0
	for (var i = 0; i < it; i++)
		timesum += executionTime(command, n, t)
	return timesum / it
}

function executionTimeArr(command, it){
	var arr = []
	for (var n = NSTART; n < N; n++) {
		let arr1 = [n]
		console.log(n)
		for (var t = TSTART; t < T; t++) {
			arr1.push(executionTimeAvg(command, Math.pow(2,n), t, it))
		}
		arr.push(arr1)
	}
	return arr
}

function zort(command, it){
	var arr = executionTimeArr(command, it)
	return arr.map(function (line) {
		return line.map(v => v.toString().replace(/\./gi, ',').slice(0, 9)).join(SEPARATOR)
	}).join("\n")
}

function bench(command) {
	var title = command
	for (var t = TSTART; t < T; t++) {
		title += SEPARATOR + t
	}
	var z = zort(command, 10)
	return title + "\n" + z
}

var result = [bench("./stealing"), bench("./sharing")].join("\n")

fs.writeFile("./bench.csv", result, function(err) {
    if(err) {
        return console.log(err);
    }
    console.log("The file was saved!");
}); 

