List of features I would like to build 
1) Weather layer and all the associated hid communcation required (https://gist.github.com/fauxpark/03a3efcc7dbdfbfe57791ea267b13c55)
2) Rotary Encoder velocity check. EX: on scroll function a slow turn is 1 line, medium turn 5 lines, fast turn 10 lines.  Idea to do this: If each encoder event counts as a keypress, on turn set a timer and count keypress within n MS, if 1 press - 1 line, 2 press - 5 line, 3 press 10 line etc..   There may be a more elegant way of handling this. (https://github.com/itsnoteasy/forgotten_dreams) 
3) OLED - introudce a cool image to display on idle for a duration of a minute following 30 seconds of inavtivity (and then screen off)  https://qiita.com/guri3/items/844675b637c88515a989
