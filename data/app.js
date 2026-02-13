const API="/api/data"

let lastSeen=Date.now()

async function fetchData(){

try{

const r=await fetch(API)

if(!r.ok)throw 0

const d=await r.json()

updateUI(d)

lastSeen=Date.now()

wifi.className="badge green"

}
catch{

wifi.className="badge red"

}

}


function updateUI(d){

power.innerText=d.power.toFixed(1)

voltage.innerText=d.voltage.toFixed(1)

currentLPM.innerText=d.currentLPM.toFixed(1)

todayEnergy.innerText=d.todayEnergy.toFixed(2)


pumpBadge.innerText=d.pump_running?"PUMP ON":"PUMP OFF"

pumpBadge.className="badge "+(d.pump_running?"green":"red")


tankBadge.innerText=d.tank_full?"FULL":"FILLING"

tankBadge.className="badge "+(d.tank_full?"green":"gray")


updateTank(d.waterLevel)


lastUpdate.innerText=new Date().toLocaleTimeString()

}


function updateTank(arr){

let level=arr.length*12.5

tankFill.style.height=level+"%"

levelText.innerText=level+"%"

}


setInterval(fetchData,2000)

fetchData()
