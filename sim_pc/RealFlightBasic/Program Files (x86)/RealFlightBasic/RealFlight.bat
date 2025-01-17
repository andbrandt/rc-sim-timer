// SimTimer MUST signal [LEFT_ARROW] [LEFT_ARROW] [LEFT_ARROW] [Alt+F4] before calling this bat file
// SimTimer MUST signal [WIN] "c:\Program Files (x86)\RealFlightBasic\RealFlight.bat" [RETURN] to invoke this bat file

copy RealFlightBasic_simTimerStart.ini "C:\Users\%USERNAME%\Documents\RealFlight Basic\RealFlightBasic.ini"
"c:\Program Files (x86)\RealFlightBasic\RealFlight.exe"
