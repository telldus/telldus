/* 
Copyright (C) 2009 by Michael Khalili

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

function TimezoneDetect(){
    var dtDate = new Date('1/1/' + (new Date()).getUTCFullYear());
    var intOffset = 10000; //set initial offset high so it is adjusted on the first attempt
    var intMonth;
    var intHoursUtc;
    var intHours;
    var intDaysMultiplyBy;
 
    //go through each month to find the lowest offset to account for DST
    for (intMonth=0;intMonth<12;intMonth++){
        //go to the next month
        dtDate.setUTCMonth(dtDate.getUTCMonth() + 1);
 
        //To ignore daylight saving time look for the lowest offset.
        //Since, during DST, the clock moves forward, it'll be a bigger number.
        if (intOffset > (dtDate.getTimezoneOffset() * (-1))){
            intOffset = (dtDate.getTimezoneOffset() * (-1));
        }
    }
 
    return intOffset;
}

//Find start and end of DST
function DstDetect(){
    var dtDstDetect = new Date();
    var dtDstStart = '';
    var dtDstEnd = '';
    var dtDstStartHold = ''; //Temp date hold
    var intYearDayCount = 732; //366 (include leap year) * 2 (for two years)
    var intHourOfYear = 1;
    var intDayOfYear;
    var intOffset = TimezoneDetect(); //Custom function. Make sure you include it.
 
    //Start from a year ago to make sure we include any previously starting DST
    dtDstDetect = new Date()
    dtDstDetect.setUTCFullYear(dtDstDetect.getUTCFullYear() - 1);
    dtDstDetect.setUTCHours(0,0,0,0);
 
    //Going hour by hour through the year will detect DST with shorter code but that could result in 8760
    //FOR loops and several seconds of script execution time. Longer code narrows this down a little.
    //Go one day at a time and find out approx time of DST and if there even is DST on this computer.
    //Also need to make sure we catch the most current start and end cycle.
    for(intDayOfYear = 1; intDayOfYear <= intYearDayCount; intDayOfYear++){
        dtDstDetect.setUTCDate(dtDstDetect.getUTCDate() + 1);
 
        if ((dtDstDetect.getTimezoneOffset() * (-1)) != intOffset && dtDstStartHold == ''){
            dtDstStartHold = new Date(dtDstDetect);
        }
        if ((dtDstDetect.getTimezoneOffset() * (-1)) == intOffset && dtDstStartHold != ''){
            dtDstStart = new Date(dtDstStartHold);
            dtDstEnd = new Date(dtDstDetect);
            dtDstStartHold = '';
 
            //DST is being used in this timezone. Narrow the time down to the exact hour the change happens
            //Remove 48 hours (a few extra to be on safe side) from the start/end date and find the exact change point
            //Go hour by hour until a change in the timezone offset is detected.
            dtDstStart.setUTCHours(dtDstStart.getUTCHours() - 48);
            dtDstEnd.setUTCHours(dtDstEnd.getUTCHours() - 48);
 
            //First find when DST starts
            for(intHourOfYear=1; intHourOfYear <= 48; intHourOfYear++){
                dtDstStart.setUTCHours(dtDstStart.getUTCHours() + 1);
 
                //If we found it then exit the loop. dtDstStart will have the correct value left in it.
                if ((dtDstStart.getTimezoneOffset() * (-1)) != intOffset){
                    break;
                }
            }
 
            //Now find out when DST ends
            for(intHourOfYear=1; intHourOfYear <= 48; intHourOfYear++){
                dtDstEnd.setUTCHours(dtDstEnd.getUTCHours() + 1);
 
                //If we found it then exit the loop. dtDstEnd will have the correct value left in it.
                if ((dtDstEnd.getTimezoneOffset() * (-1)) != (intOffset + 60)){
                    break;
                }
            }
 
            //Check if DST is currently on for this time frame. If it is then return these values.
            //If not then keep going. The function will either return the last values collected
            //or another value that is currently in effect
            if ((new Date()).getTime() >= dtDstStart.getTime() && (new Date()).getTime() <= dtDstEnd.getTime()){
                return new Array(dtDstStart,dtDstEnd);
            }
 
        }
    }
    return new Array(dtDstStart,dtDstEnd);
}