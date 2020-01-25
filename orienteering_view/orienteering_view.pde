/*
  OpenSource Orienteering / Base viewer
  
  This sketch reads from serialport all the data 
  retrieved by the Arduino in the base station, 
  parse them to extract datetime info and then shows
  them into a map.
  
  It's a easy and not-parametric-at-all sketch, so next
  improvements will/should be:
  - parse recursively the info from Arduino
  - define "a priori" array / dictionary with stations coordinates so
    it's easy to place them into the map
  - maybe be able to load programmatically the map according to the stations' 
    coordinates 
  
  CC BY-SA
*/

import processing.serial.*;

Serial myPort;  // Create object from Serial class
String longRow = ""; 
String st1, st2, ora1, ora2, min1, min2, gg1, gg2, mm1, mm2, yy1, yy2;

PFont f;
PImage bg;

boolean drawPoint1, drawPoint2;

void setup() 
{
  size(1282, 806 );
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = "/dev/ttyUSB0"; 
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');

  bg = loadImage("map.jpg");
  f = loadFont("BitstreamVeraSans-Roman-14.vlw");
}

void draw()
{
  textFont(f);
  background(bg);
  //background(255);             // Set background to white
  fill(0);
  if (drawPoint1) {
    int x1 = 841;
    int y1 = 315;
    fill(0, 0, 255, 50);
    ellipse(x1, y1, 20, 20);
    fill(0);
    text(gg1 + "/" + mm1 + "/" + yy1, x1 + 50, y1);
    text(min1 + ":" + ora1, x1 + 50, y1 + 20);
  }
  if (drawPoint2) {
    int x2 = 633;
    int y2 = 417; 
    fill(0, 255, 0, 50);
    ellipse(x2, y2, 20, 20);
    fill(0);
    text(gg2 + "/" + mm2 + "/" + yy2, x2 + 50, y2);
    text(min2 + ":" + ora2, x2 + 50, y2 + 20);
  }
  textAlign(CENTER);
  textFont(f);
  //  println(mouseX, mouseY);
}
void serialEvent(Serial p)
{
  longRow = myPort.readString();
  //substring 1-12 per capire se 62 0 63
  String checkRow = longRow.substring(0, 11);
  String shortRow = "";
  /*
  if (checkRow.indexOf("0") > 1) {
   //prima lanterna
   print("ID: ");
   shortRow = longRow.substring(14, 32).replace(" ", "");
   println(shortRow);
   //println(st1 + " " + gg1 + " " + mm1 + " " + yy1 + " " + ora1 + " " + min1);
   }
   */

  if (checkRow.indexOf("61") > 1) {
    //prima lanterna
    print("prima tappa: ");
    shortRow = longRow.substring(14, 32).replace(" ", "");
    st1 = shortRow.substring(0, 2);
    gg1 = shortRow.substring(2, 4);
    mm1 = shortRow.substring(4, 6);
    yy1 = shortRow.substring(6, 8);
    ora1 = shortRow.substring(8, 10);
    min1 = shortRow.substring(10, 12);
    //println(shortRow);
    st1 = "" + Integer.parseInt(st1, 16);
    gg1 = "" + Integer.parseInt(gg1, 16);
    mm1 = "" + Integer.parseInt(mm1, 16);
    yy1 = "" + Integer.parseInt(yy1, 16);
    ora1 = "" + Integer.parseInt(ora1, 16);
    min1 = "" + Integer.parseInt(min1, 16);
    println(st1 + " " + gg1 + " " + mm1 + " " + yy1 + " " + ora1 + " " + min1);
    drawPoint1=true;
  }
  if (checkRow.indexOf("62") > 1) {
    //seconda lanterna
    print("seconda tappa: ");
    shortRow = longRow.substring(14, 32).replace(" ", "");
    st2 = shortRow.substring(0, 2);
    gg2 = shortRow.substring(2, 4);
    mm2 = shortRow.substring(4, 6);
    yy2 = shortRow.substring(6, 8);
    ora2 = shortRow.substring(8, 10);
    min2 = shortRow.substring(10, 12);
    //println(shortRow);
    st2 = "" + Integer.parseInt(st2, 16);
    gg2 = "" + Integer.parseInt(gg2, 16);
    mm2 = "" + Integer.parseInt(mm2, 16);
    yy2 = "" + Integer.parseInt(yy2, 16);
    ora2 = "" + Integer.parseInt(ora2, 16);
    min2 = "" + Integer.parseInt(min2, 16);
    println(st2 + " " + gg2 + " " + mm2 + " " + yy2 + " " + ora2 + " " + min2);
    drawPoint2=true;
  }
  /*
           etcetera
   */
}

void keyPressed() {
  if (key == 'r') {
    drawPoint1 = false;
    drawPoint2 = false;
  }
}

