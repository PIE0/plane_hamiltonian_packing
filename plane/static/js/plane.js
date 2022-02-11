var POINT_DIAM = 20;
var PLANE_HEIGHT = $(plane).height();
var PLANE_WIDTH = $(plane).width();
var CANVAS_HEIGHT = Math.ceil(PLANE_HEIGHT/POINT_DIAM)+1;
var CANVAS_WIDTH  = Math.ceil(PLANE_WIDTH/POINT_DIAM)+1;

function chap(x) {
	console.log(x);
}

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
function points_catow(points, xoffset, yoffset) {
	wpoints = [];
	for(var i=0; i < points.length; i++) {
		var point = ctow(catoc(points[i]));
		point.x += xoffset;
		point.y += yoffset;
		wpoints.push(point);
	}
	return wpoints;
}

var mark = new Array(CANVAS_HEIGHT);
for(var i=0; i < CANVAS_HEIGHT; i++) {
	mark[i] = new Array(CANVAS_WIDTH);
	for(var j=0; j < CANVAS_WIDTH; j++)
		mark[i][j] = false;
}
var circles = [];
var circle_objects = [];
var hamil_paths = [];

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
	circle_objects.push(circle);
}

document.getElementById("run").onclick = function() {run_algorithms()};
document.getElementById("clear_paths").onclick = function() {clear_paths()};
document.getElementById("clear_points").onclick = function() {clear_points()};

function clear_paths() {
	for(var i=0; i < hamil_paths.length; i++)
		hamil_paths[i].remove()
	hamil_paths = []
}

function clear_points() {
	for(var i=0; i < circle_objects.length; i++)
		circle_objects[i].remove();
	circle_objects = []
	circles = []
}

function show_paths(ret_data) {
	console.log(ret_data)
	rnd_rot = ret_data.rnd_rot;
	four_perm = ret_data.four_perm;
	zigzag = ret_data.zigzag;
	genetic = ret_data.genetic;
	fitness = ret_data.fitness
	var paths;
	if(rnd_rot != undefined)
		paths = rnd_rot;
	else if(four_perm != undefined)
		paths = four_perm;
	else if(zigzag != undefined)
		paths = zigzag;
	else if(genetic != undefined)
		paths = genetic;
	hamil_paths.push(new Path({
		segments: points_catow(paths[0], 0, 3),
		strokeWidth: 2,
		strokeColor: 'green',
		strokeJoin: 'round',
	}))
	hamil_paths.push(new Path({
		segments: points_catow(paths[1], 0, 0),
		strokeWidth: 2,
		strokeColor: 'blue',
		strokeJoin: 'round',
	}))
	document.getElementById("self_intersection").innerText = fitness[0];
	document.getElementById("shared_edges").innerText = fitness[1];
	document.getElementById("intersections").innerText = fitness[2];
	document.getElementById("final_fitness").innerText = fitness[3];
}

function run_algorithms() {
	// circles = [[0, 0], [3, 3]];
	var algorithm = document.querySelector('input[name="algorithm"]:checked').value;
	var weights = [parseInt(document.getElementById("self_intersection_weight").value),
				   parseInt(document.getElementById("shared_edge_weight").value),
				   parseInt(document.getElementById("intersection_weight").value)]
    $.ajax({
		url : 'http://127.0.0.1:8000/run/',
		headers: { 'X-CSRFToken': Cookies.get('csrftoken')},
		type : 'POST',
		data : JSON.stringify({
			points: circles,
			algorithm: algorithm,
			weights: weights,
		}),
		dataType: 'json',
		success: function(data){
			clear_paths();
			show_paths(data);
		},
		error: function (jqXHR, exception) {
			alert('Error! See js logs for more information.');
			console.log(exception);
			console.log(jqXHR);
		}
	});
}

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
