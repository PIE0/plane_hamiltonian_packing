var POINT_DIAM = 20;
var PLANE_HEIGHT = $(plane).height();
var PLANE_WIDTH = $(plane).width();
var CANVAS_HEIGHT = Math.ceil(PLANE_HEIGHT/POINT_DIAM)+1;
var CANVAS_WIDTH  = Math.ceil(PLANE_WIDTH/POINT_DIAM)+1;

function makePoint(loc) {
	if(loc.x != undefined)
		return loc;
	if(loc[0] != undefined)
		return {x: loc[0], y: loc[1]}
	assert;
}
function to_ar(loc) {
	return [loc.x, loc.y];
}
function wtoc(loc) { // canvas window coordinate to canvas array coordinate
	loc = makePoint(loc);
	return {
		x: Math.floor((loc.x+POINT_DIAM/2) / POINT_DIAM),
		y: Math.floor((loc.y+POINT_DIAM/2) / POINT_DIAM),
	}
}
function ctow(loc) { // canvas array coordinate to canvas window coordinate
	loc = makePoint(loc);
	return {
		x: Math.round(loc.x) * POINT_DIAM,
		y: Math.round(loc.y) * POINT_DIAM,
	}
}
function round(loc) {
	return ctow(wtoc(loc));
}

var ORIGIN_CANVAS_WIDTH = Math.round(CANVAS_WIDTH/2);
var ORIGIN_CANVAS_HEIGHT = Math.round(CANVAS_HEIGHT/2);
var X_AXIS_HEIGHT = ctow({x:0, y:ORIGIN_CANVAS_HEIGHT}).y;
var Y_AXIS_WIDTH = ctow({x:ORIGIN_CANVAS_WIDTH, y:0}).x;
function ctoca(loc) { // canvas array coordinate to cartesian coordinate
	loc = makePoint(loc);
	return {
		x: loc.x - ORIGIN_CANVAS_WIDTH,
		y: CANVAS_HEIGHT - loc.y - ORIGIN_CANVAS_HEIGHT,
	}
}
function catoc(loc) {
	loc = makePoint(loc);
	return {
		x: loc.x + ORIGIN_CANVAS_WIDTH,
		y: CANVAS_HEIGHT - loc.y - ORIGIN_CANVAS_HEIGHT,
	};
}
function points_catow(points) {
	wpoints = [];
	for(var i=0; i < points.length; i++)
		wpoints.push(ctow(catoc(points[i])));
	return wpoints;
}

var mark = new Array(CANVAS_HEIGHT);
for(var i=0; i < CANVAS_HEIGHT; i++) {
	mark[i] = new Array(CANVAS_WIDTH);
	for(var j=0; j < CANVAS_WIDTH; j++)
		mark[i][j] = false;
}
var circles = [];

// Draw a circle for every mouse click
function onMouseDown(event) {
	loc = round(event.point);
	canvas_loc = wtoc(loc);
	if(mark[canvas_loc.y][canvas_loc.x])
		return;
	mark[canvas_loc.y][canvas_loc.x] = true;
	var coords = ctoca(canvas_loc);
	circles.push([coords.x, coords.y]);
	var circle = new Path.Circle(loc, POINT_DIAM/2-2);
	circle.strokeColor = 'red';
	circle.strokeWidth = 3;
}

document.getElementById("run").onclick = function() {run_algorithms()};

function show_paths(paths) {
	console.log(paths);
	rnd_rot = paths.rnd_rot;
	four_perm = paths.four_perm;
	if(rnd_rot != undefined) {
		console.log(rnd_rot[0]);
		new Path({
			segments: points_catow(rnd_rot[0]),
			storkeWidth: POINT_DIAM/2,
			strokeColor: 'green',
		});
	}
}

function run_algorithms() {
    $.ajax({
		url : 'http://127.0.0.1:8000/run/',
		headers: { 'X-CSRFToken': Cookies.get('csrftoken')},
		type : 'POST',
		data : JSON.stringify({
			points: circles,
		}),
		dataType: 'json',
		success: function(data){
			show_paths(data);
		},
		error: function (jqXHR, exception) {
			alert('Error! See js logs for more information.');
			console.log(exception);
			console.log(jqXHR);
		}
	});
}

//[[2, 3], [3, 4], [5, 7], [-2, -9]]

// Draw 2D Cartesian grid
for(var i = 0; i < CANVAS_HEIGHT; i++)
	for(var j = 0; j < CANVAS_WIDTH; j++) {
		var c = new Path.Circle(ctow([j, i]), 1);
		c.fillColor = 'gray';
	}
// Draw X Axis
new Path({
	segments: [[0, X_AXIS_HEIGHT], [PLANE_WIDTH, X_AXIS_HEIGHT]],
	strokeColor: 'black',
	storkeWidth: 3,
});
for(var i = 0; i < CANVAS_WIDTH; i++) {
	new Path({
		segments: [[ctow([i, 0]).x, X_AXIS_HEIGHT+2], [ctow([i, 0]).x, X_AXIS_HEIGHT-2]],
		strokeColor: 'black',
	});
	if(i % 5 == ORIGIN_CANVAS_WIDTH % 5)
		new PointText({
			point: [ctow([i, 0]).x-POINT_DIAM/4, X_AXIS_HEIGHT+POINT_DIAM*0.8],
			content: ctoca([i, 0]).x,
			fontSize: POINT_DIAM/2,
		});
}
// Draw Y Axis
new Path({
	segments: [[Y_AXIS_WIDTH, 0], [Y_AXIS_WIDTH, PLANE_HEIGHT]],
	strokeColor: 'black',
	storkeWidth: 3,
});
for(var i = 0; i < CANVAS_HEIGHT; i++) {
	new Path({
		segments: [[Y_AXIS_WIDTH-2, ctow([0, i]).y], [Y_AXIS_WIDTH+2, ctow([0, i]).y]],
		strokeColor: 'black',
	});
	if(i % 5 == ORIGIN_CANVAS_HEIGHT % 5 && i != ORIGIN_CANVAS_HEIGHT)
		new PointText({
			point: [Y_AXIS_WIDTH-POINT_DIAM*1.1, ctow([0, i]).y+POINT_DIAM/4],
			content: ctoca([0, i]).y,
			fontSize: POINT_DIAM/3*2,
		});
}

// Select the path, so we can see its handles:
// path.fullySelected = true;
// var copy = path.clone();
// Smooth the segments of the copy:
// copy.smooth();
