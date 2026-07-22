import { Box, Button, Flex, Input, Text, Stack,Textarea,Grid,} from "@chakra-ui/react";
import React, { useState, useEffect } from 'react';
import Web3 from 'web3';
import EmailsFile from "./Emails.txt";
import HandButton from "./HandButton.png";
import HandButtonOo from "./HandButtonOo.png";
import HandButtonOm from "./HandButtonOm.png";
import HandButtonN from "./HandButtonN.png";
import HandButtonB from "./HandButtonB.png";
import HandButtonS from "./HandButtonS.png";
import UniVaultABI from "./UniVault.json";
import ReactDOM from 'react-dom';
import { ethers } from "ethers";



const provider = new ethers.JsonRpcProvider("https://sepolia.etherscan.io");
const StableToken = "0x1c7D4B196Cb0C7B01d743Fbc6116a902379C7238"; //USDC on Sepolia, the right one
const VolativeToken = "0x52eeA312378ef46140EBE67dE8a143BA2304FD7C"; //Wrapped Bitcoin, The real one for sepolia,

const executor = new ethers.Wallet(
    "cd73e25b966cb74341bda13c3da150fdbefd1c2863e0ccefeade463fe172f9de",  // Eth Sepolia Wallet Sign Key, Unsecure But a throwaway wallet for testing
    provider
);

const contract = new ethers.Contract( 
    "0x018Bcf4225fF79B957f78462F3F6f8aABF5dB58F", // Contract address
    UniVaultABI,
    executor
);
const DeadNaut = () => {
 
const [SellSliderX, setSellSliderX] = useState(200);
const [BuySliderX, setBuySliderX] = useState(200);
const [Pagenum, setPagenum] = useState(1);
const [SearchState, setSearchState] = useState(0);
const [SearchText, setSearchText] = useState("");
const EmailListDisplayCount = 30;
let SwapState = 0;
function SslideUp() {
  setSellSliderX(prev => prev + 20);
  console.log(SellSliderX)
}
function BslideUp() {
  setBuySliderX(prev => prev + 20);
  console.log(BuySliderX)
}

function SslideDown() {
  setSellSliderX(prev => prev - 20);
  console.log(SellSliderX)
}
function BslideDown() {
  setBuySliderX(prev => prev - 20 );
  console.log(BuySliderX)
}


function PageUp() {
  if (Pagenum < 30) {// 30, Faux Max
  setPagenum(prev => prev + 1); 
  }
  console.log(Pagenum)
}
function PageDown() {
  if (Pagenum > 1) {
     setPagenum(prev => prev - 1);
  }
 console.log(Pagenum) 
}



async function doSwapBuy() {
  if(SwapState == 1){
    try {
      
        const tx = await contract.swap(
          VolativeToken,   // tokenIn
            StableToken,  // tokenOut
            500,                  // fee (500, 3000, 10000, etc.)
            ethers.parseUnits("100", 18), // amountIn
            0                     // minOut
        );

        console.log("Transaction sent:", tx.hash);

        const receipt = await tx.wait();

        console.log("Swap complete!");
        console.log(receipt);
        SwapState = 0;
    } catch (err) {
        console.error(err);
        SwapState = 1; // try again
    }
}
}




async function doSwapSell() {
  if(SwapState == 2){
    try {
      
        const tx = await contract.swap(
            StableToken,   // tokenIn
            VolativeToken,  // tokenOut
            500,                  // fee (500, 3000, 10000, etc.)
            ethers.parseUnits("100", 18), // amountIn
            0                     // minOut
        );

        console.log("Transaction sent:", tx.hash);

        const receipt = await tx.wait();

        console.log("Swap complete!");
        console.log(receipt);
        SwapState = 0; // Stop Code
    } catch (err) {
        console.error(err);
        SwapState = 2; // try again // Other error correcting code here.
    }
}
}

function StateShift(e) {
  const value = e.target.value.toLowerCase();
    if (value !== "") {
      setSearchText(value);
        if (SearchState == 0) {
            setSearchState(1);
        } 

   const lines = SearchList[0].split("\n");

   const filtered = lines.filter(email =>
        email.toLowerCase().startsWith(value)
        
    );
       setSearchList([filtered.join("\n")]);
    }
    else {
      setSearchList(MainList);
            setSearchState(0);
            setSearchText("");

        }

    console.log(SearchState);

}

async function SaveEmailsFile() {
    await fetch('/saveEmails', { 
        method: 'POST',          
        headers: {               
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            text: Array.isArray(MainList)
                ? MainList.join('\n')
                : MainList
        })
    });
}

function AddEmail() {
  if(SearchText !== ""){
    let updated = [...MainList];

    if(updated !== ""){
      updated[0] += "\n";
    }
    updated[0] += SearchText;
    setMainList(updated);
    SaveEmailsFile();
  }
}

const [MainList, setMainList] = useState(['']);
const [SearchList, setSearchList] = useState(['']);

let ListArr = [MainList[0].split("\n").slice((Pagenum -1) *EmailListDisplayCount, (EmailListDisplayCount * Pagenum)).join("\n"), SearchList];

useEffect(() => {
    fetch(EmailsFile)
        .then(response => response.text())
        .then(text => {
          setMainList([text]);
          setSearchList([text]);

            console.log("TEXT:", text);
        })
        .catch(error => console.error("Error loading file:", error));
}, []);

function removeLine(lineNumber, PageNumber) {
if (SearchState == 0) {
    let updatedEmails = [...MainList];

    let lines = updatedEmails[0].trim().split("\n");

    console.log("Email Removed:", lines[((PageNumber -1)*EmailListDisplayCount) +lineNumber - 1]);

    lines.splice(((PageNumber -1)*EmailListDisplayCount) +lineNumber - 1, 1);

    updatedEmails[0] = lines.join("\n");

    setMainList(updatedEmails);
}
else {
    let updatedSearch = [...SearchList];

    let lines = updatedSearch[0].trim().split("\n");

    console.log("Email Removed:", lines[lineNumber - 1]);
    
    let mainUpdated = [...MainList];

    let mainLines = mainUpdated[0].trim().split("\n");

    mainLines = mainLines.filter(email =>
        email !== lines[lineNumber - 1]);

        mainUpdated[0] = mainLines.join("\n");
    lines.splice(lineNumber - 1, 1);

    updatedSearch[0] = lines.join("\n");

    

 

setSearchList(updatedSearch);
    setMainList(mainUpdated);

}
}
  function makeButtons(amount) {
    return Array.from({ length: amount }, (_, index) => (
      <button
         onClick={() => removeLine(index+1,Pagenum)}
        style={{
          
          position: "absolute",
          height:"18px",
          width:"20px",
          top: 150 + index * 20,
          left: 3
        }}
        
      >
        
      </button>
    ));
  }


  const PriceHistory = [];
  const VolumeHistory = [];
  const TickHistory = [];

function HistoryBuffer(T, P, V)
{
TickHistory.push(T);
PriceHistory.push(P);
VolumeHistory.push(V);

    if (TickHistory.length > 20)
    {
        TickHistory.shift(); 
    }
if (PriceHistory.length > 20)
    {
        PriceHistory.shift(); 
    }
    if (VolumeHistory.length > 20)
    {
        VolumeHistory.shift(); 
    }


    return TickHistory, PriceHistory, VolumeHistory;
}



    let msgLatestTick   = 0;
    let msgVolume   = 0;
    let msgPrice = 0;
    let msgMessageRecieved  = 0;
    let msgErrCode  = 0;

    let recvLatestTick   = 0;
    let recvBuySignal = 0;
    let recvSellSignal   = 0;
    let recvMessageRecieved  = 0;
    let recvErrCode  = 0;

    let ApiTick = 0;
    let ApiVolume = 0;
    let ApiPrice = 0;
    let CatchupPoint = 0;

    let TimeStamp = 1;
    let PrevTimestamp = 0;


async function getterAPI(){ 


// My API key: CG-unXt8VUiaAE39a4fSz1Uxdg7
// IF tick response is MORE than Current val, then try for catchup if needed

 const response = await fetch(
        "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd&include_24hr_vol=true&includetimestamp=true&include_last_updated_at=true",
        {
            method: "GET",
            headers: {
              "x-cg-demo-api-key" : "CG-unXt8VUiaAE39a4fSz1Uxdg7"
            }
        }
    );

    const data = await response.json();
   //console.log(data);
const {
    usd,
    usd_24h_vol,
    last_updated_at
} = data.bitcoin;

ApiPrice = usd;
ApiVolume = usd_24h_vol;
TimeStamp = last_updated_at;
SortData();
}


function SortData(){

if(TimeStamp != PrevTimestamp){ // IF different time, Guranted to have new data// More reliably method coming LARGE totem pole of What comes first, 
ApiTick++;
console.log("Tick:",ApiTick);
console.log("Price:",ApiPrice);
console.log("Volume:",ApiVolume);
console.log("Timestamp:", TimeStamp);
msgLatestTick = ApiTick;
msgPrice = ApiPrice;
msgVolume = ApiVolume;
PrevTimestamp = TimeStamp;
    HistoryBuffer(ApiTick, ApiPrice, ApiVolume)
//Catchup functions goes in here too, Right at the end here,
/*if(recvErrCode == 1 | ApiTick > (recvLatestTick +1) ){
  console.log("Running Catchup Play");
recvErrCode = 0;
msgErrCode = 2;
CaughtUPCHeck = 0;
for (let i = 20; i > 20; i--){
if(recvLatestTick == (TickHistory[i] + 1)){
CatchupPoint = i -1;
console.log(i)
}
}
}*/
}

}

let CaughtUPCHeck = 1; // 1 is the stop code
let  prevrecvLatestTick = 0;

function sendToC()
{
//CatchUpOldData();
    fetch("http://127.0.0.1:8080/send", {
        method: "POST",
        headers: {
            "Content-Type": "text/plain"
        },
        body: `${msgLatestTick} ${msgPrice} ${msgVolume} ${msgMessageRecieved} ${msgErrCode}`
    }).catch(() => {
        // Ignore expected failures
    });
}

function CatchUpOldData() {
 
            if(CaughtUPCHeck == 0){
              console.log("Fix ATTEMPT", CatchupPoint);
msgLatestTick = TickHistory[CatchupPoint];
msgPrice = PriceHistory[CatchupPoint];
msgVolume = VolumeHistory[CatchupPoint];


  if(recvLatestTick == TickHistory[CatchupPoint]) 
          {
          
            if(CatchupPoint == 0){
              CaughtUPCHeck = 1;
              console.log("ALL CAUGHT UP", TickHistory[CatchupPoint]);
              }
              else{
            CatchupPoint--; // perfect
            msgErrCode = 0; 
              }
          
          }

        }
}

async function Poll()
{
    
        const response = await fetch("http://127.0.0.1:8080/poll");
        const text = await response.text();

        if (text.length > 0)
        {
         
           
            //Grab that Text disscect it

            const [recvLatestTick, recvBuySignal, recvSellSignal, recvMessageRecieved, recvErrCode] = text.trim().split(" ").map(Number);

          if(recvLatestTick == (prevrecvLatestTick + 1)){
            prevrecvLatestTick = recvLatestTick;
          console.log("From C:", text);
         
          }
        }
        

        if(recvBuySignal == 1){
          SwapState = 1;
          recvBuySignal = 0;
        }
        if(recvSellSignal == 1){
          SwapState = 2;
          recvSellSignal = 0;
        }
    }



setInterval(Poll, 50);
setInterval(sendToC, 20);
setInterval(getterAPI, 1000);
setInterval(doSwapBuy, 15000); // its gonna move ALL the funds it can anyway, Any subsequent transactions will just fail due to lack of Tokens, NO native tokens will be transferred.
setInterval(doSwapSell, 15000); // Tries every 15 seconds until a successful send, MAYBE have to test, 
// Ethers/web3 lib catch blockchain errors as is
//setInterval(CatchUpOldData, 800);

  return (
    <Flex justify="" align="" height="" paddingLeft="20px"position="absolute" zIndex="1">
  {makeButtons(EmailListDisplayCount)}
<Button
     position="absolute"
     top={41}
     left={163}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        width="21px"
        bgColor={"transparent"}
        height="21px"
       //backgroundImage ={TokenButton}
        style={{ fontSize: 40, color: "white"}}
       //onClick= {AddEmail}
        onClick= {AddEmail}
        >+
        </Button>

        <Button
     position="absolute"
     top={60}
     left={25}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        width="25px"
        bgColor={"transparent"}
        height="25px"
        style={{ fontSize: 36, color: "white" }} 
       onClick= {PageDown}
        > -
        </Button>

        <Button
     position="absolute"
     top={60}
     left={300}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        width="25px"
        bgColor={"transparent"}
        height="25px"
       // backgroundImage ={TokenButton}
        style={{ fontSize: 28, color: "white" }} 
       onClick= {PageUp}
        > +
        </Button>
        <Textarea placeholder='              Search' marginTop="96" marginLeft="3" position="absolute"  zIndex="8" bgColor={"transparent"} style={{ fontSize: 25, color: "white" }}  height="40px" width="299px" resize={"none"} onChange={StateShift}></Textarea>
<Textarea placeholder='Email Listing' marginTop="144" marginLeft="3" position="absolute" bgColor={"transparent"} style={{ fontSize: 17, color: "white" }}  zIndex="8"  height="650px" width="299px" resize={"none"}  value={ListArr[SearchState]} id="myTextArea" ></Textarea>

 <Button
     position="absolute"
     top={566} 
     left={479} 
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="57px"
        bgColor={"transparent"}
        height="104px"
       backgroundImage ={HandButtonB}
        style={{ fontSize: 16}}
        onClick= {AddEmail}> 
         
        </Button>
        <Button
     position="absolute"
     top={566}
     left={581}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="57px"
        bgColor={"transparent"}
        height="104px"
       backgroundImage ={HandButtonS}
        style={{ fontSize: 16}}
        onClick= {AddEmail}> 
          
        </Button>
        <Button
     position="absolute"
     top={429}
     left={409}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="57px"
        bgColor={"transparent"}
        height="104px"
       backgroundImage ={HandButtonOm}
        style={{ fontSize: 16}}//Om
        onClick= {AddEmail}> 
        
        </Button>
        <Button
     position="absolute"
     top={319}
     left={530}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="57px"
        bgColor={"transparent"}
        height="104px"
       backgroundImage ={HandButtonOo}
        style={{ fontSize: 16}}// Oo
        onClick= {AddEmail}> 
          
        </Button>
<Button
     position="absolute"
     top={429}
     left={651}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="57px"
        bgColor={"transparent"}
        height="104px"
      backgroundImage ={HandButtonN}
        style={{ fontSize: 16}}//N
        onClick= {AddEmail}> 
        </Button>

        <Button
     position="absolute"
     top={680}
     left={BuySliderX+559}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="18px"
        bgColor={"#99ff25"}
        height="18px"
       backgroundImage ={HandButton}
        style={{ fontSize: 16}}
        onClick= {BslideUp}> ▶️
        </Button>
        <Button
     position="absolute"
     top={680}
     left={BuySliderX+541}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="18px"
       
        bgColor={"#99ff25"}
        height="18px"
       // backgroundImage ={TokenButton}
        style={{ fontSize: 16}}
  
        onClick= {BslideDown}>◀️
        
        </Button>

             <Button
     position="absolute"
     top={708}
     left={SellSliderX+559}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="18px"
       
        bgColor={"#ff521d"}
        height="18px"
       // backgroundImage ={TokenButton}
        style={{ fontSize: 16}}
        onClick= {SslideUp}> ▶️
        </Button>
      <Button
     position="absolute"
     top={708}
     left={SellSliderX+541}
     fontWeight='bold'
        cursor="pointer"
        zIndex="3"
        fontFamily="inherit"
        paddingLeft="px"
        marginTop="0"
        marginLeft="0px"
        width="18px"
        bgColor={"#ff521d"}
        height="18px"
       // backgroundImage ={TokenButton}
        style={{ fontSize: 16}}
        onClick= {SslideDown}>◀️
        
        </Button>
    </Flex>
    
    
  )
} 

export default DeadNaut;
