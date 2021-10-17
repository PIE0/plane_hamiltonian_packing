
var POINT_DIAM = 5;
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
		x: Math.round(loc.x / POINT_DIAM),
		y: Math.round(loc.y / POINT_DIAM),
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
	circles.push(canvas_loc);
	var circle = new Path.Circle(loc, POINT_DIAM/2-2);
	circle.strokeColor = 'red';
	circle.strokeWidth = 3;
}

// Draw 2D Cartesian grid
for(var i = 0; i < CANVAS_HEIGHT; i++) {
	var color = 'gray';
	new Path({
		segments: [ctow([0, i]), ctow([CANVAS_WIDTH, i])],
		strokeColor: color,
		dashArray: [1, POINT_DIAM-1],
	});
}

new Path({
	segments: [[0, X_AXIS_HEIGHT], [PLANE_WIDTH, X_AXIS_HEIGHT]],
	strokeColor: 'black',
	storkeWidth: 3,
});
for(var i = 0; i < CANVAS_WIDTH; i++) {
	new Path({
		segments: [[ctow([i, 0]).x, X_AXIS_HEIGHT+3], [ctow([i, 0]).x, X_AXIS_HEIGHT-3]],
		strokeColor: 'black',
	});
	if(i % 5 == ORIGIN_CANVAS_WIDTH % 5)
		new PointText({
			point: [ctow([i, 0]).x-4, X_AXIS_HEIGHT+15],
			content: ctoca([i, 0]).x,
			fontSize: 12,
		});
}

new Path({
	segments: [[Y_AXIS_WIDTH, 0], [Y_AXIS_WIDTH, PLANE_HEIGHT]],
	strokeColor: 'black',
	storkeWidth: 3,
});
for(var i = 0; i < CANVAS_HEIGHT; i++) {
	new Path({
		segments: [[Y_AXIS_WIDTH-3, ctow([0, i]).y], [Y_AXIS_WIDTH+3, ctow([0, i]).y]],
		strokeColor: 'black',
	});
	if(i % 5 == ORIGIN_CANVAS_HEIGHT % 5 && i != ORIGIN_CANVAS_HEIGHT)
		new PointText({
			point: [Y_AXIS_WIDTH-20, ctow([0, i]).y+5],
			content: ctoca([0, i]).y,
			fontSize: 12,
		});
}

// Select the path, so we can see its handles:
// path.fullySelected = true;
// var copy = path.clone();
// Smooth the segments of the copy:
// copy.smooth();
