#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>   
#include <winsock2.h> // Only windows only Lib
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#define NOMINMAX
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
// SC stands for Sync Chart ITS PERCENT activation rate accross ALL relevant Hands, Buy or Sell
  // DEFAULT DEFINEMENT //
#define HandTotal 24
#define PriceChartDisplayRange  172
#define PriceChartMemMax  1000
#define PricePlointGap 3
#define HSRPlointcount  40 // actually 1 less than input number, Because 0 is first number here
#define HSRPlointGap 22
#define HSRPlointHeight  0
#define MaxHeight 333
#define MAX_DOTS 1024
double SlidingTextXVal = 1445; //Full width - 32( Art block width)
double SlideCounter = 0;
double SlideRate = 3;
int SlideMax = 1;
double SlideMaxAdd = 0;
int SRChartUpdatelog = 0;
int PriceChartUpdatelog = 0;
int PriceChartSTARTINGPOINT = 0;
int SRChartTempVal = 0;
int TradeCount = 0;
double PriceChartTempVal = 0;
char ScrollingNames[HandTotal];
char pattern[50];
int currentFrame = 0;
int StartingTick = 0;
int LatestTick = 0;
int MaxTickDelay = 10;
int GlobalPTick = 0;
int AlgoTick = 0;
 int prevAlgoTick = 0;
double GlobalPrice = 0;
double GlobalVolume = 100.00;
float Climax = 00.00; //Percent Nearness to closing on a sell or buy, Will be converted to float later,

double HighestPrice = 0;
double LowestPrice = 0;
float BuyPnt = 1000.00;
float SellPnt = 1000.00;
bool RLMODE = 0; // The indicator of IF we are looking to buy next or sell next, Zero means buy,
float BoughtPrice = 0;
float SoldPrice = 0;
int RPercentSelected = 50.00;
int LPercentSelected = 50.00;// some settables
int FocusWheelSetting = 0;
int runonce = 0;
int RMaxWeight = 0;
int RScore = 100.00;
int RTotalActive = 0;
int RTotalUnready; //needing more data to determine active state
char RactiveIDs[HandTotal * 4];
char RpreviousActiveIDs[HandTotal * 4];
int LMaxWeight = 0;
float LScore = 100.00;
int LTotalActive = 0;
int LTotalUnready; //needing more data to determine active state
char LactiveIDs[HandTotal * 4];                                                                                                               // Before  After
char LpreviousActiveIDs[HandTotal * 4]; // When all 1,000 are implemented, Have changecheck Make a Organized entire text file of the  list in a  #####  #####
bool Buy = false;                                                                                                                         // #####  #####
bool Sell = false;                                                                                                                        // #####  #####  style format, Before and AFter, Top to bottom
bool newQuit = false;
bool BookQuit = false;
int lastHoveredId = -1;
SDL_FRect RedSliderKnob = {1222, 540, 9, 27};   
SDL_FRect BlueSliderKnob = {1222, 580, 9, 27};
int BuyPoint = 0;
int BluesliderStartX = 1222;  // leftmost x position
int BluesliderMaxRange = 200;
bool BluedraggingSlider = false;
int SellPoint = 0;
int RedsliderStartX = 1222;  // leftmost x position
int RedsliderMaxRange = 200;
bool ReddraggingSlider = false;
bool wateranimationComplete = false;
int wateranimationframe = 0;
int WaterClockSpeed = 0;
int flicker = 0;
int flickerRate = 15;
bool Toggle = true;
int Page = 0;
int PageStatus = 0; // its for the button
int PageState;
int RanktoRender = 0;
int Plointct = 0;
float TimeSpreadf = 0;
float PriceSpreadf = 0;
float TimeMagnitudef = 0;
int CurrentPage = 0;
float HoldTimer = 500;
int CycleCT = 0;
int PerformenceLogBookmark = 0;
const int FrameWidth = 29;
const int FrameHeight = 25;
const int FrameCount = 6;
SDL_FRect Plointdest[10];
SDL_FRect Plointsrc ={0,0,9,9};
SDL_FRect RankLoc ={270,40,33,33};
SDL_FRect RankSrc ={0,0,33,33};
int PlointCTStatus = 0;
int TimeSpreadStatus = 0;
int PriceSpreadStatus = 0;
int TimeMagnitudeStatus = 0;
int PlointMoving = 0;
int prevplaceholder = 0;
SDL_FRect PlointCTLeft = {505,75, 24,16};
SDL_FRect PlointCTRight = {560,75, 24,16};
bool loopActive = false;
bool Simul = false;
bool BookOpen = false;
bool ConnectionActivated = false;
int InitOnce = 0;
int PurelyVisualPrettiness = 9;
bool ApplyAndSaveHTA = false;
 SDL_Window *BOOK = NULL;
SDL_Renderer *Bookrenderer = NULL;
SDL_Texture *BookTexture = NULL;
SDL_Texture *PlointTexture = NULL;
SDL_Texture *BookTextTexture= NULL;
SDL_Texture *BookBasicTexture= NULL;
SDL_Texture *BookCoverTexture = NULL;
SDL_Texture *HRankSTexture = NULL;
SDL_Texture *HRankATexture = NULL;
SDL_Texture *HRankBTexture = NULL;
SDL_Texture *HRankCTexture = NULL;
SDL_Texture *HRankDTexture = NULL;

SDL_Surface *BookCoverSurface = NULL;
SDL_Surface *BookBasicSurface = NULL;
SDL_Surface *bookSurface = NULL;
SDL_Surface *BookText = NULL;
SDL_Surface *PlointSurface = NULL;
SDL_Surface *HRankS = NULL;
SDL_Surface *HRankA = NULL;
SDL_Surface *HRankB = NULL;
SDL_Surface *HRankC = NULL;
SDL_Surface *HRankD = NULL;
   // DEFAULT DEFINEMENT //











typedef struct {
    SDL_FRect rect;
    char id;
    int RH; // Render Height
    bool isActivated;
    int height;
    Uint32 lastHoverTime;
} Dot;


 Dot dots[MAX_DOTS];

// Create a dot and add it to the array.


typedef struct  
{
 int Boost;
 bool Active;  
}Virtue; // the 5 virtues, These highlight certain hands(trading techniuqes) depending on the your Trading enviroment, Low Risk, High risk(HUGE oversimplification, The risk is always the same)
//Code in at what point is it acceptable to buy the asset
//based on percent approval
//Have the Settings for the app be a text doc, so BClass = Bsum, and B sum 
// equals whatever is input at that entry into the text doc, 
//Functions to edit all the settings individually in your own personal text doc,
//The code generates the text doc with preset sums ill make up later
    
typedef struct  
{  

Virtue TrueNeutral;
Virtue Human;
Virtue Bomb;
Virtue Babel;
Virtue Industry;

}TheFiveVirtues;
TheFiveVirtues BoostDefault;

typedef struct{
   char set[100];
} Setting; 

// Structs ^
// Void    v


void CopyBoosts(TheFiveVirtues* dist, const TheFiveVirtues* src) {
    dist->Babel.Boost = src->Babel.Boost;
    dist->Bomb.Boost = src->Bomb.Boost;
    dist->Human.Boost = src->Human.Boost;
    dist->Industry.Boost = src->Industry.Boost;
    dist->TrueNeutral.Boost = src->TrueNeutral.Boost;
}


    const char *filenames[] = {
        "HandLog.txt", // Hand Activation and Deactivation Change log
        "PriceLog.txt", 
        "PerformenceLog.txt",
        "FreeSlot.txt" 
    };
void createDumpFiles() {

    int numFiles = sizeof(filenames) / sizeof(filenames[0]);
    FILE *file;

    for (int i = 0; i < numFiles; ++i) {
        // Check if file exists
        file = fopen(filenames[i], "a");
        if (file != NULL) {
            printf("File '%s' already exists. Skipping.\n", filenames[i]);
            fclose(file);
        } else {
            file = fopen(filenames[i], "w");
            if (file == NULL) {
                fprintf(stderr, "Failed to create file: %s\n", filenames[i]);
            } else {
                printf("Created file: %s\n", filenames[i]);
                fclose(file);
            }
        }
    } 
    fclose(file);

}




void appendToFile(const char *filename, const char *format, ...) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file for appending: %s\n", filename);
        return;
    }

    // Get current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", t);

    // Write timestamp to file
    fprintf(file, "%s ", timestamp);

    // Handle variable arguments and write formatted text
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    // Add newline
    fprintf(file, "\n");

    fclose(file);

}

void ChangeCheck(char *current, char *previous, size_t bufferSize) {
    if (strcmp(current, previous) != 0) {
        appendToFile("HandLog.txt", 
                     "Active IDs changed from %.*s to %.*s",previous,current);
printf("Active IDs changed from %.s to %.s\n", previous, current);
        strncpy(previous, current, bufferSize);
    }
    
    strcpy(current, "");
}

void SaveFiles(const char *files[4]) {
    char timestamp[32];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", t);

    // Create folder named after timestamp
    if (mkdir(timestamp) != 0) {
        perror("mkdir failed");
        return;
    }

    for (int i = 0; i < 4; i++) {
        char new_filename[512];
        snprintf(new_filename, sizeof(new_filename), "%s/%s_%s", timestamp, timestamp, files[i]);

        if (rename(files[i], new_filename) != 0) {
            perror("rename failed");
        } else {
            printf("Moved %s to %s\n", files[i], new_filename);
        }
    }
}




typedef struct {
    double price;
    float height;
    SDL_FRect location;
} Ploint;

Ploint BuySRateChart[HSRPlointcount];
Ploint SellSRateChart[HSRPlointcount];
Ploint DisplaySRateChart[HSRPlointcount];
Ploint ActivationPercentageGauge[2];
Ploint PriceChart[PriceChartMemMax];
SDL_FRect PriceChartScrollButton = {394,3,520,520};
SDL_FRect PriceChartScrollsTATus = {890,33,20,20};
typedef struct  
{
 char Name[12];
 int Weight;
}WeightClass;


typedef struct {
    int IDNUM;
    char ID[12]; // Roman Numeral/ text based number
    char Name[50]; //Flashy Title
    char Descriptor[500];
    WeightClass Weight; //Flag Weight
    int PageType;
    float ActivationRate;// Percent to activation 
    bool Activation; //Whether the Flag is Raised/Activated 0 or 1
    TheFiveVirtues virt;
} Hand;




typedef struct{
    int Tick;
    float Price; // Dollar amount, Why? just because.
    float TrueRelativity; // The Ploints RElativity to the starting Point, Its TRUE designation
    int PlointFound; // 1 is found, 0 if not yet found
    int HigherLower; // 1 for higher, 0 for lower, gets sorted at runtime, Looped like all the other settings.
    // if Pricearr[0] > Pricearr[1]
    // pricearr[1].HigherLower = 0, It compares to the past val what IT is, and so on
} Prices;

typedef struct {
Prices PriceArr[10]; // The "Prices" are logarithmic Relative Percentage changes, percent change from 1 to 100,
int Times[9]; //TIme magnitude just effects how the numbers get put here, The number of pixels X TMag = the Time
int Size; //# of ploints in arr
int NextTicktoCheck; // Checks the Current Price if this val is NULL, then updates this val as the Current Tick # + 1, if NULL after that, Then the price feed stopped, Error "Price Input Halted," My app has no Delays except fo rvisuals purely, It moves at the rythem of the data recieved
int StartingTick; // THE PLACE the first ploint starts, So our origin isnt lost and ALL data is checked.
int TimeSpread; // Spread will never exceed 100// Meaning the app would ahve a weakness to shitcoins that easily multi x // True
int PriceSpread;  // in the GUI element its a float, we want it all to be Ints, That level of variability is handled by the spread aspect
int LastPlointFound;
int TimeMagnitude;
}HandTypePlointArch;

HandTypePlointArch Two;
HandTypePlointArch Three;
HandTypePlointArch Seven;
HandTypePlointArch EightTeen;

HandTypePlointArch* RelativeLocationA[4] = {&Two, &Three, &Seven, &EightTeen};
int RelativeLocationB[4] = {2,3,7,18};// The correlated page numbers. its a sorting list



void SaveSetting(const char* filename, int lineNumber, const char* text)
{
    if (lineNumber < 1)
        return;

    FILE* file = fopen(filename, "r");
    if (!file)
    {
      printf("File doesnt exist, Needs to created BY YOU %s", filename);
    }

    char** lines = NULL;
    size_t count = 0;
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file))
    {
        char** temp = realloc(lines, sizeof(char*) * (count + 1));
        if (!temp) return;

        lines = temp;
        lines[count++] = strdup(buffer);
    }

    fclose(file);

    // Expand file if needed
    if (lineNumber > count)
    {
        char** temp = realloc(lines, sizeof(char*) * lineNumber);
        if (!temp) return;

        lines = temp;

        for (size_t i = count; i < lineNumber; i++)
            lines[i] = strdup("\n");

        count = lineNumber;
    }

    // Replace requested line
    free(lines[lineNumber - 1]);

    size_t len = strlen(text);
    lines[lineNumber - 1] = malloc(len + 2);
    sprintf(lines[lineNumber - 1], "%s\n", text);

    // Write back
    file = fopen(filename, "w");
    if (!file) return;

    for (size_t i = 0; i < count; i++)
    {
        fputs(lines[i], file);
        free(lines[i]);
    }

    free(lines);
    fclose(file);
}

void SavetoHandLog(int StructLoc){ // Appp is corrupting here, Before the entry, But by the exit of this func
                                    // its partially corrupted the struct.
// The Struct should already be properly updated, Save button already hit. That happened down below, Save it to its correct place in the text doc
char HTABuffer[100] = "";
char HandTAFormat[100] = "";

HandTypePlointArch* Chosen = RelativeLocationA[StructLoc];

snprintf(HTABuffer, sizeof(HTABuffer),"%d %d %d %d ", Chosen->Size, Chosen->TimeSpread, Chosen->PriceSpread, Chosen->TimeMagnitude);
strcat(HandTAFormat,HTABuffer);
strcpy(HTABuffer,"");

for (size_t y = 1; y < Chosen->Size; y++) // I should be able to let it process the start too, It should all equal zero,, But I wont. Skipping it entirely is cleaner.
{
//The first point is the start, its vals are all ZEro, Its shouldnt be being read, Its a known stable position    
snprintf(HTABuffer, sizeof(HTABuffer),"%d %.0f %d ", Chosen->PriceArr[y].HigherLower, Chosen->PriceArr[y].TrueRelativity, Chosen->Times[y-1] );
//printf("HL %d", Chosen->PriceArr[y].HigherLower);
 strcat(HandTAFormat,HTABuffer);
 strcpy(HTABuffer,"");
}


SaveSetting("ArcTypeHANDs.txt",StructLoc +1,HandTAFormat);

//printf("HERE %s\n",HandTAFormat);
}



void FetchHandLog(int StructLoc)
{
    HandTypePlointArch* Chose = RelativeLocationA[StructLoc];
       printf("Struct Location # %d\n", StructLoc);
    
    if (StructLoc + 1 < 1)
        return;

    FILE *file = fopen("ArcTypeHANDs.txt", "r");
    if (!file)
        return;

    char line[512] = "";
    int currentLine = 0;

    while (fgets(line, sizeof(line), file))
    {
        currentLine++;

        if (currentLine == StructLoc + 1)
        {
            int NumsSorted = 0;
            char NumSortBuffer[200] = "";
            char tempBuf[200] = "";
            char NumSor[200] = "";
            char tempBu[200] = "";
for (int z = 0; z < 100; z++)
{
            if (NumsSorted <= 3)
            {
               if (line[z] != ' ')
               {
                tempBuf[0] = line[z];
                tempBuf[1] = '\0';
                strcat(NumSortBuffer,tempBuf); // Only captures one letter, Strange. Maybe a loop UNTIL Z coord equals ' ' aka Spacebar
                strcpy(tempBuf,"");
               }
                else{
                switch (NumsSorted)
                {
                case 0:
                    Plointct = atoi(NumSortBuffer);
                    Chose->Size = atoi(NumSortBuffer);
                    strcpy(NumSortBuffer, "");
                   // printf("Size/PlointCT %d\n",Chose->Size);
                    NumsSorted++; 
                    break;
                    case 1:
                    TimeSpreadf = atof(NumSortBuffer); // make sure this correlates correctly to how the data is saved in teh sext file, may need to swap this and pSpreadf
                    Chose->TimeSpread = atoi(NumSortBuffer);
                    strcpy(NumSortBuffer, "");
                  //  printf("TimeSpread %d\n",Chose->TimeSpread);
                    NumsSorted++; 
                    break;
                    case 2:
                    PriceSpreadf = atof(NumSortBuffer);
                    Chose->PriceSpread = atoi(NumSortBuffer);
                    strcpy(NumSortBuffer, "");
                  //  printf("PriceSpread %d\n",Chose->PriceSpread);
                    NumsSorted++; 
                    break;
                    case 3:
                    TimeMagnitudef = atof(NumSortBuffer);
                    Chose->TimeMagnitude = atoi(NumSortBuffer);
                    strcpy(NumSortBuffer, "");
                   //printf("TimeMagnitude %d\n",Chose->TimeMagnitude);
                    NumsSorted++; 
                    break;
                default:
                    break;
                }
                }
              // Do the dots here and find otu why they  no longer render,
            }
            
        
            
//printf("HL After %d Z:%d\n", Chose->PriceArr[1].HigherLower, z);
if (NumsSorted == 4)
{
for (int i = 1; i < Chose->Size; i++)
    {
        for (int L = 0; L < 3; L++)
        {
             for (int q = 0; q < 5; q++)// max 5 digits for the data input, Can be increased, just a memory usage thing
            {z++;
                if (line[z] != ' ')
               {
                tempBu[0] = line[z];
                strcat(NumSor,tempBu); // Only captures one letter, Strange. Maybe a loop UNTIL Z coord equals ' ' aka Spacebar
                strcpy(tempBu,"");
               }
                else{
             switch (L)
            {
            case 0:
                 Chose->PriceArr[i].HigherLower = atoi(NumSor);
                 strcpy(NumSor, "");
                // printf("HigherLower %d\n",Chose->PriceArr[i].HigherLower);
                break;
            case 1:
                Chose->PriceArr[i].TrueRelativity =atof(NumSor);
                strcpy(NumSor, "");
               //  printf("TrueRelativity %.3f\n",Chose->PriceArr[i].TrueRelativity);
                break;
            case 2:
                Chose->Times[i-1] = atoi(NumSor);
                strcpy(NumSor, "");
                // printf("Times %d\n",Chose->Times[i]);
                break;
            default:
                break;
            }
            break;
            }
        }}
    }
    break;
}

}

              
for (int e = 0; e < 10; e++)
{
Plointdest[e].x = 394 + (e * 10);
Plointdest[e].y = 205;
Plointdest[e].w = 9;
Plointdest[e].h = 9;
}


    for (int k = 1; k < Plointct; k++)
             {
                if (Chose->PriceArr[k].HigherLower == 1)
                {
                   Plointdest[k].y = Plointdest[k-1].y + Chose->PriceArr[k].TrueRelativity;
                 //  printf("TRUE REL %f\n",Chose->PriceArr[k-1].TrueRelativity); 
                }
                if (Chose->PriceArr[k].HigherLower == 0)
                {
                    Plointdest[k].y = Plointdest[k-1].y - Chose->PriceArr[k].TrueRelativity;
                 //   printf("TRUE REL %f\n",Chose->PriceArr[k-1].TrueRelativity);  
                }
                
               Plointdest[k].x = Plointdest[k-1].x + (Chose->Times[k-1] + PurelyVisualPrettiness);
               Plointdest[k].w = 9;
               Plointdest[k].h = 9; // can convert all of these coords into percentages so its automatically mathematically Correct with a min distance of 1 pixel, Relative to the distance of the furthest one out, aND A hard limit on the furthest one out which is the last ploint, so if its
               // 6 ploints than the 6th ploint cant go past the farthest edge, and none can go past it.
             }
             
            //printf("%s", line);  // line already contains newline
          
            fclose(file);
            break;
}
}

   
}

//takes the data in the text doc and updates the visual vals in code, not the struct
// only updates the visual because only saving can update the struct in code, and thats alright,

//HandTypePlointArch GetHandData (int Page){
// go into the HandLog.txt file, that string becomes the sums of the hands struct at startup, Use the roman NUmeral 
//ID and the Hand finder filter in the book page method to Find the Hand, Hand.ID = Name of HandTypePloint ARch we are looking for, 
//So pull the data from the code that way.
//}


double GetPriceAtTick (int Tick){ // Ill probably run my app 1 tick behind so we can reliably pluck data from our own personal text store,

double Price;
if ( Tick == LatestTick)
{
    Price = GlobalPrice; // However we store that data, Get it.
   return Price;
}


    FILE *file = fopen("PriceLog.txt", "r");
    if (file == NULL){
        printf("PriceLog File Missing\n");
        return 0.0;
    }
   
    char line[256];

    for (int i = 0; i <= Tick; i++)
    {
        if (fgets(line, sizeof(line), file) == NULL)
        {
           
            fclose(file);
            printf("Line Doesnt exist");
            return 0.0; // Line doesn't exist
        }
    }


char *numberStart = strrchr(line, ']');

if (numberStart != NULL)
{
    Price = atof(numberStart + 1); // Reads the number after the ']'
}
else
{
    Price = 0.0;
}



fclose(file);
return Price;
}

typedef struct CountOfThree{
    int Tick;
    double price;
    bool Found;
} CountOfThree;

typedef struct RuleOfThreeParts
{
 int Magnitude; // How many ticks coupled together as one to compare,
 int NextTickToCheck;
 CountOfThree CountToThree[3];
}RuleOfThreeParts;


RuleOfThreeParts One;
RuleOfThreeParts Four;
RuleOfThreeParts Five;
RuleOfThreeParts Six;

RuleOfThreeParts* RuleOfThreeBinder[4] = {&One, &Four, &Five, &Six};



void RuleOfThree( Hand* HandID,RuleOfThreeParts* ROfTHand){ // MUST be initialized NextTick to CHeck is 0, Tick vals are 0 through count to three, Magnitude default 1, First point default NOT FOUND, starts at tick 0



    //for (int Y = 0; Y < 2; Y++)
    //{ //Ensure it doesnt lag behind on Retries
    
    
    int Next = 0;
    for (int L = 0; L < 3; L++)
    {
        if ( ROfTHand->CountToThree[L].Found == false)
        {
            Next = L;
            break;
        }
        
    }
    
      
          //  printf("Point %d Cleared Check 1 for %s  AT Tick %d LOOKING FOR %d\n",i, HandID->ID,AlgoTick, ROfTHand->NextTickToCheck + ROfTHand->Magnitude );
        if (ROfTHand->NextTickToCheck + ROfTHand->Magnitude  <= AlgoTick) // If i have them re -search will need more math to source a new working starting tick
    {   //printf("Point %d Cleared Check 2 for %s  AT Tick %d\n",i, HandID->ID,ROfTHand->CountToThree[i].Tick );
      
        double price = 0;
         if (ROfTHand->CountToThree[2].Found == false)
         { //printf(" SEARCHING Check %s\n", HandID->ID);
           for (int k = 0; k < ROfTHand->Magnitude; k++)
           {
            price = GetPriceAtTick(ROfTHand->NextTickToCheck + k); // If 0 gets the first line 1 gets the seconds
            ROfTHand->CountToThree[Next].price = ((ROfTHand->CountToThree[Next].price * k) + price) / (k + 1); // Should work, if not remove parantehses on K + 1
           }


          printf("TRYING Point %d for %s AT Tick %d\n",Next, HandID->ID, ROfTHand->NextTickToCheck);
          

             if (Next == 0  && ROfTHand->CountToThree[Next].Found == false)
           {
             ROfTHand->CountToThree[Next].Found = true;
            ROfTHand->CountToThree[Next].Tick = ROfTHand->NextTickToCheck;
            ROfTHand->NextTickToCheck = ROfTHand->CountToThree[Next].Tick + ROfTHand->Magnitude;
              printf("Point %d Found for %s  AT Tick %d\n",Next, HandID->ID,ROfTHand->CountToThree[Next].Tick );
           }
           

             if (Next >= 1  && ROfTHand->CountToThree[Next].price >= ROfTHand->CountToThree[Next-1].price && ROfTHand->CountToThree[Next].Found == false )
           { 
            
            ROfTHand->CountToThree[Next].Found = true;
            ROfTHand->CountToThree[Next].Tick = ROfTHand->NextTickToCheck;
            ROfTHand->NextTickToCheck = ROfTHand->CountToThree[Next].Tick + ROfTHand->Magnitude;
              printf("Point %d Found GOOP for %s  AT Tick %d\n",Next, HandID->ID,ROfTHand->CountToThree[Next].Tick );
           }

            if (ROfTHand->CountToThree[1].Found == true)
         { printf("Rule Of Three Hand %s Found at Tick: %d \n", HandID->ID,ROfTHand->CountToThree[Next].Tick );
           HandID->Activation = true; 
           HandID->ActivationRate = 100;
         }
     
         }

       // }

         
        

         
      
 
             if (Next >= 1 && ROfTHand->CountToThree[Next].price <= ROfTHand->CountToThree[Next-1].price | ROfTHand->CountToThree[2].Found == true )
           { 
printf("Trying Again for %s  Point %d AT Tick %d WAS LOOKING THRU TO %d\n", HandID->ID, Next, ROfTHand->NextTickToCheck, ROfTHand->NextTickToCheck + ROfTHand->Magnitude );
              if (ROfTHand->CountToThree[2].Found == true)
         { /* Add to track record that it was accurate */ }
            
           printf(" Trying again %s  %d\n", HandID->ID, Next);
           for (int b = 0; b < 3; b++)
           {ROfTHand->CountToThree[b].Found = false;
            HandID->Activation = false;
            ROfTHand->NextTickToCheck = ROfTHand->CountToThree[0].Tick + ROfTHand->Magnitude;
            ROfTHand->CountToThree[b].price = 0;
            HandID->ActivationRate = 0; // Do i need to set it to zero ? Nope, but I might use the visual in the future in a way thatll make this matter.
           }
        }
           
    }


}

void HandTypePloint( Hand* HandID, HandTypePlointArch* Shape) {
   

if (Shape->StartingTick == 0) // They all start on zero even if started late, 
   {                            // May have to preload all the starting ticks with zero,
    Shape->StartingTick = AlgoTick; // Also the price store txt file has to capture and date as much data as possible, and for calculations reset the one it reads from at the start of the app, make a new file,
   }                                  // Shutdown procedure, thats a tad bit of dummy proofing, 

    Shape->PriceArr[0].PlointFound = true; // The Beginning
    Shape->PriceArr[0].Tick = Shape->StartingTick;

    double StartingPrice = GetPriceAtTick(Shape->StartingTick); // Now you gotta make sure it STARTS at the right tick, IF null start it at GlobalPtick
    for (int N = 0; N < Shape->Size; N++){
        if (Shape->PriceArr[N].HigherLower == 0)
        {
            Shape->PriceArr[N].Price = (StartingPrice / 100) * (Shape->PriceArr[N].TrueRelativity + 100); // TrueRelativity is 1% per pixel
        }
        if (Shape->PriceArr[N].HigherLower == 1) // IF its HIGHER than P0 (ploint zero aka the first) then it gets 0, if its lower, than it gets a 1, this just gives our specs a clear and firm outline, Keeps it from running away, less dynamic, But infinitely less chaotic
        {                                                   // Should be relative to the last point not specifically the first only, IT may not matter.
            Shape->PriceArr[N].Price = (StartingPrice / 100) * (100 - Shape->PriceArr[N].TrueRelativity);
        }
        if (N == 0) // Should only ever by the first point, and it has to be set somewhere so why not here
        { // the setter for the high lows will just be a for loop that starts at 1 so it skips zero
            Shape->PriceArr[N].Price = StartingPrice;
        }
    }
for (int L = 0; L < Shape->Size; L++)
{
    if (Shape->PriceArr[L].PlointFound == true)
    {
        Shape->LastPlointFound = L;
        HandID->ActivationRate = ((100 / Shape->Size) * L); // DO THIS for all Hands, Easy to forget, Its purely analytics
    }
    if (L == Shape->Size -1)
    {
        HandID->Activation = true;
       // printf("Hand Activated  Hand: %s Title: %s\n",HandID->ID, HandID->Name);
        //Play sound
        return; //Exit
    }
    
}
    for (int j = 0; j < 3; j++) {
    
    double TMax = (Shape->Times[Shape->LastPlointFound] / 100) * (Shape->TimeSpread + 100);//may bug out due to parent vals being simple ints
    double TMin = (Shape->Times[Shape->LastPlointFound] / 100) * (100 - Shape->TimeSpread); // This might crash at the last ploint because there is no time for it,. Doesnt need one,
    float PMax = (Shape->PriceArr[Shape->LastPlointFound + 1].Price / 100) * (100 + Shape->PriceSpread);
    float PMin = (Shape->PriceArr[Shape->LastPlointFound + 1].Price / 100) * (100 - Shape->PriceSpread);

    if(Shape->NextTicktoCheck == 0) {
    Shape->NextTicktoCheck = Shape->PriceArr[Shape->LastPlointFound].Tick + (TMin - 1);} //Perfect

    double Price = GetPriceAtTick(Shape->NextTicktoCheck); // Assuming price TIcks from the API are numerical and successional 1 by 1
    if(Price == 0) {
        printf("Price Not Found/Returned NULL %d\n", Shape->NextTicktoCheck);
    return;
    }
    if (Shape->NextTicktoCheck > AlgoTick) { // the + one just give us a computational delay/buffer incase of any hold ups(me ordering the func unoptimally, Some forgiveness) // I dont know what he meant by that
    printf("Next Viable Tick Not yet Arisen   Hand: %s Title: %s  Next Tick: %d\n",HandID->ID, HandID->Name,Shape->NextTicktoCheck );
    return;} 

    //This function takes in a hand struct ID(and all its members)
    //Ploint count, T for time from Ploint 1 to 2 and so on, so # of unique T vals is Ploint count -1, If not that many return error
    //Hand Struct, Ploint Layout Struct, Ploint(x,y), Times, Single Price Spread, Single Time Spread Val
    //Then in function code that makes sure Its HIGHer or lower as it should be corresponding to the original Input, Ploint 2 being higher in price than 1 etc.
    //Checks the tick, check notebook for complex alg you planned out already
    // Need that Dynamic Array from Size 2 to 10
    // FUCK doing a resizable array in C, we are gonna do a max of 10 but only read the number said to sent with the funtcion
    // Will need to be able to access all prior price ploints and current, Starting from 1 onward,
    //The end goal is to create the min max for every ploints time and price, and check if both true, If not before time expiry, Check the next starting ploint,
    // Check 3 price points at a time until the Potential Start = The most current price

if (Shape->NextTicktoCheck - Shape->PriceArr[Shape->LastPlointFound].Tick <= TMax || Shape->NextTicktoCheck - Shape->PriceArr[Shape->LastPlointFound].Tick >= TMin ){
if (Price > PMin || Price < PMax)
{
Shape->PriceArr[Shape->LastPlointFound + 1].PlointFound = true;
    printf("Ploint %d Found For Hand: %s Title: %s\n",Shape->LastPlointFound +1,HandID->ID, HandID->Name);
Shape->PriceArr[Shape->LastPlointFound + 1].Tick = Shape->NextTicktoCheck;
//HandID->ActivationRate = ((100 / Shape->Size) * (Shape->LastPlointFound + 1));
Shape->NextTicktoCheck = 0;}} // Hopefully it can check multiple ticks per call, Its starting over from zero// Will have to check if I implemented that, THis func was made in a Beautiful flurry of passion, Hesitant to touch it.

   if ( GlobalPTick - Shape->PriceArr[Shape->LastPlointFound + 1].Tick > TMax){ //IF it TIME EXPIRES
     for (int k = 1; k < Shape->Size; k++)
 {
        Shape->PriceArr[k].PlointFound = false;
 }
        Shape->LastPlointFound = 0; 
        Shape->StartingTick = Shape->StartingTick + 1;
        printf("Time Expiry for ID %s\n", HandID->ID);
        printf("Retrying %d\n", j);
    }printf("Check Shape.Size %d\n", Shape->Size);
    if (Shape->LastPlointFound == Shape->Size - 1)
    {
        HandID->Activation = true;
    }
    }
    }    // HOLD UP, the prices in PriceARR are actually all relative to the first point, and NOT relative to eachother successivley 
                                         // The First Ploints TRUE price is price of the TICK last checked, IT IS ALWAYS FOUND, so check system, To go through all PLOINTS
                                           // UNTIL one ISNT yet found, Then look for that one UNTIL it is either FOUND or Time Expired. IF FOUND, Update the struct with the tick it was found at and at what price
                                           // If found THEN time Expired, RESET ALL ploints to NOT FOUND Shape.PriceARR[k].PlointFound = false;

        /* code */ // A loop that sorts another vale attached to the "price" struct, High, Lower, uPDATES it, If Higher, Must be Higher, If Shape.
        // ALSO VERY IMPORTANT, EVERY dot on the HORIZAONTAL AXIS on the ARC creation menu is 10 ticks by default scaleable to 1000 ticks per dot with a slider, Another setting sent over in the "HandTypePLointArc" struct
        // AND the chart is relative, point 3 is 4 pixels down 50 pixels over, from point 2, etc, Can place it anywhere, But the canvas has a limited space so fit it in that
        //So I can just make the board pretty, dont have to label it so much
        // ALSO need a P and T min spread, so it doesnt just insta find the arc in a few minutes
        // a Percentage of previous values, So the spread affects the UP and the Down, If the TIme is 100 Ticks, 
        // THEN the spread is 50%, Then it must be atleast 50 ticks away, but no more than 150 ticks away, yeah
    
 
    
    
    void CreateFogMap (){

        int Start = 0;

        int centerX = 202;
        int centerY = 500;
        int dotSize = 3;
        int centerDotSize = 1;
        int Xspacing = 5;
        int  Yspacing = 3; 
        int branches = 32;
        for (int i = 0; i < 1024; i++) {
        dots[i].rect = (SDL_FRect){500, 100, 3, 3};
        }

      for (int i = 0; i < 32; i++) {
        
       for (int i = 0; i < branches; i++) {
            dots[Start].rect.x = centerX - (i * Xspacing);
            dots[Start].rect.y = centerY - (i * Yspacing);
            dots[Start].rect.w = dotSize;
            dots[Start].rect.h = dotSize;
            
            
           
            dots[Start].id = Start+1;
          //  printf("DOT PRINTED: %d \n",dots[Start].id);
            Start = Start +1;
          
       }
        
            
            centerX = centerX + Xspacing;
            centerY = centerY - Yspacing;
    }

}
            
    
     typedef struct {
        char Text[512];
        char LastMessage[512];
        int Textint[512];
        int Size;
        int LastLetter;
        int Step;
        float StartingX;
        float StartingY;
        SDL_FRect srcrect[512];
        SDL_FRect destrect[512];
     } message;

 message GUITextBox[8];
 message ToastNotif;
bool NotifStart = false;
int NotifDelay = 0;
SDL_FRect TextBackground;




message PageNumber;
message HandName;
message DescText;
message TrackRecord;
message HandTypeArcsettings;

void TypeTextToScreen (message *Message, int Size, float StartingX, float StartingY){

if (GlobalPTick == 0){
Message->StartingY = StartingY;
Message->StartingX = StartingX;
Message->Size = Size; // Is this right? only at tick 0?
}

if (Message->LastMessage != Message->Text){
  //  printf(" Message: %s\n", Message->Text);
char CapitalLEtters[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ. 1234567890*!$%";
char LowerCaseLetters[]="abcdefghijklmnopqrstuvwxyz. 1234567890*!$%";

for (int i = 0; i < strlen(Message->Text); i++){
char Letter = Message->Text[i];

for (int j = 0; j < strlen(CapitalLEtters); j++){
if (CapitalLEtters[j] == Letter) {
    Message->Textint[i] = j;
   // printf(" Val %d\n", j);
    break;}

if (LowerCaseLetters[j] == Letter) {
    Message->Textint[i] = j;
   // printf(" Val %d\n", j);
    break;}
}
}

strcpy(Message->LastMessage,Message->Text);
}

int FontHeight[] = {12, 18, 28, 48};
int LetterWidth[] = {10, 12, 20, 48};
int FontStartY[] = {0, 12, 30, 48};
int FontWidth[168] = {
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,0,10,10,
10,10,10,12,11,9,12,12,12,10,10,10,10,11,10,11,11,10,11,11,10,12,10,10,10,4,7,7,10,9,10,9,9,10,9,9,10,10,0,0,10,10,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,18,18,18,18,18,18,18,18,18,18,14,14,14,18,18,0,0,10,10,
10,10,10,10,10,10,1,10,1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,0,10,10};
for (int s = 0; s < strlen(Message->Text); s++)
{
               if (Message->Textint[Message->Step] == 39){
                 Message->StartingY = StartingY;
                 Message->StartingX = StartingX;
                 Message->Step = 0;
                 Message->LastLetter = 0;
                return;
               }

                Message->srcrect[Message->Step].y = FontStartY[Message->Size];
                Message->srcrect[Message->Step].x = LetterWidth[Message->Size] * Message->Textint[Message->Step];
                Message->srcrect[Message->Step].w = FontWidth[Message->Textint[Message->Step] + (Message->Size *42) ];
                Message->srcrect[Message->Step].h = FontHeight[Message->Size];

                Message->destrect[Message->Step].y = Message->StartingY;
                Message->destrect[Message->Step].x = Message->StartingX += Message->LastLetter;
                Message->destrect[Message->Step].w = FontWidth[Message->Textint[Message->Step] + (Message->Size *42) ];
                Message->destrect[Message->Step].h = FontHeight[Message->Size];
                //printf("Val %f\n",  Message->destrect.y);
                if (Message->Textint[Message->Step] == 38){
                Message->StartingY += FontHeight[Message->Size] +4;
                Message->StartingX = StartingX;
                Message->LastLetter = 0;
                }

                if (Message->Textint[Message->Step] != 38){
                
                 Message->LastLetter = FontWidth[Message->Textint[Message->Step] + (Message->Size *42)];
                }
Message->Step++;
}
}


 void ToastNotification (char * Notification, int Type){

    strcpy(ToastNotif.Text, Notification);

    float TextBoxWidth[] = {300, 400, 600};
    float TextBoxWidthHeight[] = {30, 200, 300};
    float StartX[] = {550, 300, 200};
    float StartY[] = {100, 100, 100};

    TextBackground.h = TextBoxWidthHeight[Type];
    TextBackground.w = strlen(Notification) * 12;
    TextBackground.x = 700 - (TextBackground.w /2); //background color deprecated
    TextBackground.y = StartY[Type];
    NotifStart = true;

TypeTextToScreen(&ToastNotif,1,TextBackground.x+7,TextBackground.y+7);

};



void SlidingText (char *ID){ // PERFECT now add the text Images and Update the WIDTH per image used, Order the String, And COnvert the String into a string of immages, Done before
// If called with no ID value, it just cycles the text around, 
// If called with ID value sent it resets the X value to 1
if (ID == 0) 
{
 SlidingTextXVal -= SlideRate;
// SlideMaxAdd = SlidingTextWVal / SlideRate; //FIx this later, Longer it is, The longer it goes to dissappear
//SlideMax += SlideMaxAdd;
 if (SlidingTextXVal < SlideMax)
 {
   SlidingTextXVal = 1445;
   //SlidingTextXVal = SlidingTextXVal + 1445 + 10 * SlideNameCounter;
   // Search the list of last active Hands IF they are still active everytime it resets to the other side of the screen
   
 }
}
else{

snprintf(pattern, sizeof(pattern), ".%s.", ID);
if (strstr(ScrollingNames, pattern) != NULL) {
//printf("Would be a Duplicate, Skipping..\n");
return;}
if (SlideCounter == 0)
{strcpy(ScrollingNames, ".");} // the text thats supposed to go in here, Is the LATEST to be activated, So ChangeCheck, Subtract the difference, Send that off to this display,
strcat(ScrollingNames, ID); 
strcat(ScrollingNames, ".");
//strcat(ScrollingNames, temp);
printf("Names Scroll %s\n", ScrollingNames);// So if we search for the exact string Including the borders like this ",IX," we wont find false positives like LIX
SlideCounter++; // Takes in the ID, Add , and , On both sides of it IF its the first sent Else just ",ID"
if (SlideCounter == 20)
{strcpy(ScrollingNames, "");
SlideCounter = 0;}
}

} //This Function is Complete, ORders the Names, and pass it 20 names itll reset to blank and refill next turning, IT has no filter, But that in verison 3.45(a while from now)



void UpdateSRChart(float value) {
    SRChartUpdatelog = HSRPlointcount -1; 
    
if (value > 100)
{
   value = 100.00f;
}


            for (int q = 0; q < SRChartUpdatelog; q++) // q should stop at 38, log at 39
            {
        if (RLMODE == 0)
        {   
          BuySRateChart[q].height = BuySRateChart[q+1].height;
        }
        else{
          SellSRateChart[q].height = SellSRateChart[q+1].height;
        }
            }
            
        if (RLMODE == 0)
        {
            BuySRateChart[SRChartUpdatelog].height = value;
            //printf("%f B mode \n", BuySRateChart[SRChartUpdatelog].height);
        }
        else{
            SellSRateChart[SRChartUpdatelog].height = value;
            //printf("%f S mode\n", SellSRateChart[SRChartUpdatelog].height);
        }
           
            
       //  SRChartTempVal = RLMODE ? BuySRateChart[SRChartUpdatelog].height: SellSRateChart[SRChartUpdatelog].height;

}

void UpdatePriceChart(double value) {
    
    //printf("%d ", PriceChartUpdatelog);
        if (value != 0) // If the value is NOT PURE 0 which crypto or stocks never is, else just update the chart relativity
{
        PriceChartTempVal = value;
        PriceChartUpdatelog = PriceChartMemMax;
        LowestPrice = 100000000000000; // if we even encounter a price over a trillion, this breaks, that does happen its just rare
        HighestPrice = 0;
        for (int i = 0; i < 1000; i++)
        {
                PriceChart[i].price = PriceChart[i+1].price;
                //printf(" Highest  %d\n ",i);
        }
        PriceChart[PriceChartMemMax-1].price = PriceChartTempVal;
//printf(" Highest PRrice AFTER %.6lf and ChartPrice %lf %d\n ", value, PriceChart[PriceChartMemMax].price, PriceChartTempVal);

}
//printf(" Highest PRrice AFTER %.6lf and ChartPrice %lf %d\n ", value, PriceChart[PriceChartUpdatelog].price, PriceChartUpdatelog );
for (int p = (PriceChartMemMax - PriceChartSTARTINGPOINT) - PriceChartDisplayRange; p <  PriceChartMemMax - PriceChartSTARTINGPOINT;  p++){
if (PriceChart[p].price > HighestPrice) {
    
HighestPrice = PriceChart[p].price;
}

if (PriceChart[p].price < LowestPrice) {
    if (PriceChart[p].price==0)
    {
        PriceChart[p].price = 0; // NO NULL available so 0 is my placeholder val, But 0 is by far the lowest val, So we have to ignore
    }
    else{
    LowestPrice = PriceChart[p].price;
    }

}
}

for (int p = (PriceChartMemMax - PriceChartSTARTINGPOINT) - PriceChartDisplayRange; p <  PriceChartMemMax - PriceChartSTARTINGPOINT;  p++) {
double PeakVariance = (HighestPrice - LowestPrice);
//printf(" Peak Variance Highest %.2lf  Lowest %.2lf\n", HighestPrice, LowestPrice); 
double RelativePricing = (100/PeakVariance) * (PriceChart[p].price - LowestPrice);
PriceChart[p].height = (3.33 * ((100/PeakVariance) * (PriceChart[p].price - LowestPrice))); // Current price minus lowest price As a percentage of Peak Variance, Multiply that by 3.33 100% aka it is the highest price 333 pixels tall
if (PriceChart[p].price == 0){ PriceChart[p].height = (3.33 * 0);};
//printf(" Price.h %.3lf\n",PriceChart[p].height);
//printf(" Peak Variance %.6lf\n", PeakVariance); // BEFORE all the data is fulled the NUll read prices make the chart midline heavy, Not really something that needs fixing, Ill leave it as a way to remind myself, For cosmetic reasons it should be, But I wont, It displays correctly
PriceChart[p].location.h =  -(PriceChart[p].height + 44); // The reason the height changes is because at the start theres 0 vals in the chart, Which makes all numbers that do come in relative to zero near the highest val. The code is perfect

}
if (value != 0)
{ 
  PriceChartUpdatelog++;
}
}

void SimulatePriceChange(double *p, double *v)
{
    static float Pricedata[24000];
    static float Voldata[24000];
    static int index = 0;
    static int loaded = 0;
//printf("Price %.0lf\n", *pez); // error here, Occaisonally stops at 30 and goes back down, Wait, The ENd of the file, DUH
    if (!loaded)
    {
        FILE *f = fopen("SimuData.txt", "r");
        if (!f) return;

        for (int i = 0; i < 24000; i++)
        {
            fscanf(f, "%f,%f", &Pricedata[i],&Voldata[i]);
        }
 
        fclose(f);
        loaded = 1;
    }

    *p = Pricedata[index];
    *v = Voldata[index];

    index++;
    if (index >= 24000)
        index = 0;
}





// ? ? ?  NETWORKING   ? ? ? //
bool IsMessageRecieved = 0;
int TransmissionCT = 0;


int msgLatestTick = 0;
int msgBuySignal = 0;
int msgSellSignal = 0;
int msgMessageRecv = 0;
int msgErrCode = 0;

int recvLatestTick = 0;
double recvVolume = 0;
double recvPrice = 0;
int recvMessageRecv = 0;
int recvErrCode = 0;

int PREVrecvLatestTick = 0;
int PREVrecvVolume = 0;// these bottom 4 are kinda useless atm, May come in handy for error handling, Like I said earlier, Dont know what kinda errors we are gonna run into if any, Nothing wrong with my code, 
int PREVrecvPrice = 0; // May have to compensate if theres something wrong with theirs, Leaving this here until further notice.
int PREVrecvMessageRecv = 0;
int PREVrecvErrCode = 0;

SOCKET serverSocket = INVALID_SOCKET;
int ServerRunning = 0;
char Prevbody[512] = "";

void HandleHTTPRequest(SOCKET clientSocket)
{
    char buffer[4096];

    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0)
        return;

    buffer[bytes] = '\0';

    //printf("\n--- RAW REQUEST ---\n%s\n", buffer);

    // -----------------------------
    // 1. Detect request type
    // -----------------------------
    int isPOST = strstr(buffer, "POST /") != NULL;
    int isGET  = strstr(buffer, "GET /") != NULL;

    // -----------------------------
    // 2. Extract BODY (POST data)
    // -----------------------------
    char *body = strstr(buffer, "\r\n\r\n");
    if (body)
    {
        body += 4;

        if (isPOST)
        {
            fflush(stdout);
          if (body[0] != '\0')
{
    

    sscanf(body, "%d %lf %lf %d %d", &recvLatestTick,&recvPrice,&recvVolume,&recvMessageRecv,&recvErrCode);
   if (Simul == false)
   {
     if (LatestTick < recvLatestTick)
    {LatestTick = recvLatestTick;
    LatestTick = recvLatestTick;
    GlobalPrice = recvPrice;
    UpdatePriceChart(GlobalPrice);
    GlobalVolume = recvVolume;
    msgLatestTick = LatestTick;
    LatestTick =  recvLatestTick;
    msgLatestTick = LatestTick;
    // IF the API tick data is TRUE to the all time reference of the chart, This can stay as it is, If it is not and is only relative to the data they store, Than this needs to be restructered to a App startup Time relative sorting method,
    PREVrecvVolume = GlobalVolume; // and the IF we are behind on data can stay relative to the API as well, As thats a Short Time reference, We dont know what we may run into, My Algo cycling without truly new data would dilute its calculation.
    PREVrecvPrice = GlobalPrice; 
    GlobalPrice = recvPrice;
    GlobalVolume = recvVolume;

         printf("LATEST DATA %s\n", body);
         char BufferGPrice[10] = "";
          snprintf(BufferGPrice, sizeof(BufferGPrice),"%lf",recvPrice);// Will have to do for Volume also later, But not Right now. Manually clear for now, Because testing phase, But auto Clear at startup o rshutdown
         appendToFile("PriceLog.txt", BufferGPrice);
         strcpy(BufferGPrice, "");
        
    }
   }
   
   
   
// The data should never be corrupted, Negative numbers, Decimals, Improper number of membors, Integer break, Many things to check for, IF it Fails, And I faith it wont, We will adapt to the challenges the world gives me, Rather than infinitely Preparing for a challenge that may never come
// Message recieved is ONLY for Buy signal or Sell Signal on the Data in Side, (here)
if (recvMessageRecv == 1)
{
    IsMessageRecieved = true;
}




if (recvLatestTick > (LatestTick + MaxTickDelay)) //
{
   msgErrCode = 1; // ERror Code 1 is for IF we are behind on data,  // Tell the WEB api, To store all price points  from here on out, and feed them back to us until we are caught backup
   // If error code triggered, OVeride the Tick and PRice val sent over, Incrementing with each tick val sent TO the site from the C app, as it catches up until its caught up, So it goes Price API function, Then ERROR code catchup Function, Skips if no error code,
   // API func does its thing regardless, Catchup Modulates it after the fact, If Recv tick On Website Side == LatestAPITick -1 // Or -2 idk yet. -2 just to be safe.  
   //recvErrCode responding with 2 is a confirmation That it recieved our Error Code 1, and to set it back to zero
   printf("ERROR: App Behind on Price Data\n");
}
if ( recvErrCode >= 1 && recvErrCode == msgErrCode * 2)
{
    msgErrCode = 0; // The website doesnt send us any error codes, That doesnt make sense.
    // Tho we can send error codes both ways with the same 2 data slots using a Evens and Odds systems, theres just no need.
    printf("ERROR CODE Transmit SUCCESSFUL, STATUS: GREEN\n");
}



}
           
            
        }
    }

    // -----------------------------
    // 3. Handle keep-alive safely
    // -----------------------------
    int keepAlive = strstr(buffer, "keep-alive") != NULL;

    // Always force simple HTTP behavior (IMPORTANT)
    const char *connHeader = keepAlive
        ? "Connection: keep-alive\r\n"
        : "Connection: close\r\n";

    // -----------------------------
    // 4. Build response
    // -----------------------------
    char response[1024];

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "%s"
        "\r\n"
        "DATA (%d %d %d %d %d)",
        connHeader,
        msgLatestTick,
        msgBuySignal,
        msgSellSignal, // works, But isnt handling Closesocker properly, Poll is still polling tho, So IDK what the cause is, Just eventually breaks.
        msgMessageRecv, 
        msgErrCode
    );

    // -----------------------------
    // 5. Send response
    // -----------------------------
    //printf("Responding");
    send(clientSocket, response, strlen(response), 0);

    // -----------------------------
    // 6. Close ONLY if not keep-alive
    // -----------------------------
    if (!keepAlive)
   {
        closesocket(clientSocket);
   }
}

DWORD WINAPI HTTPThread(LPVOID lpParam)
{
    while (1)
    {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == INVALID_SOCKET)
            continue;

        HandleHTTPRequest(clientSocket);

       closesocket(clientSocket);
    }

    return 0;
}

int HTTP_StartServer(void)
{
    if (ServerRunning)
        return 1;

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
        return 0;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET)
        return 0;

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        return 0;

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    ServerRunning = 1;

    printf("HTTP server listening on port 8080\n");

    return 1;
}






// ? ? ?  NETWORKING   ? ? ? //

bool PerformanceRating = 0; // Good/Bad
int MedianWaitTime = 1;
int OldData = 0;// Skip Past NUmber, If already evaluated, Dont weigh new data down with it Skip past all that are already read IF used to make a BAD determination
int BadSequencesInARow = 0; // Update this when Olddata gets a value Old data being the number of lines to skip,
int TimesChecked = 0;
int TimeToWait = 0; // This will eventually just cause the app to stop itself if it makes more than like 9 bad trades in a row, Which I think is fair, 
        //  Possible Longer Wait if Losing // If I wanna see app performance, Compare the Apps Excitement to the price chart, They should match,Thats already coded into the app, The data just isnt saved in a longterm format
        
void JudgePerformence(){
    // Reads the performence log line by line, Only Stores Profit or Loss IN a Percent format
    // And the time it was Sold aka Locked in at that profit, PLUS the Min wait it was set to, 
    // If more than 3 or more trades, And bad, Is bad, If bad and LEss, Not bad.
    // MUST read all data available to assess not just the first three we have, Read until string empty
    // This part reads the text log and Updates median wait and badsequences in a row


    FILE *txtfile = fopen("PerformenceLog.txt", "r");
    if (!txtfile){return;}
        

    char line[512] = "";
    int currentLine = 0;

    while (fgets(line, sizeof(line), txtfile))
    {
        currentLine++;

        if (currentLine >= PerformenceLogBookmark)
        {
            int NumsSorted = 0;
            char Date[25] = ""; 
            char PerfSortBuffer[200] = "";
            char tempBuf[200] = "";
            char tempBu[200] = "";
            float PnL = 0;
            int Time = 0;
            float TempPnL = 0;
            int TempTime = 0;
for (int z = 0; z < 100; z++)
{
            if (NumsSorted < 3)
            {
               if (line[z] != ' ')
               {
                tempBuf[0] = line[z];
                tempBuf[1] = '\0';
                strcat(PerfSortBuffer,tempBuf); // Only captures one letter, Strange. Maybe a loop UNTIL Z coord equals ' ' aka Spacebar
                strcpy(tempBuf,"");
               }
                else{
                switch (NumsSorted)
            {
            case 0:
                
                if ((currentLine - PerformenceLogBookmark)>= 1)
                {
                    TempPnL = atof(PerfSortBuffer);
                    PnL = (((currentLine - PerformenceLogBookmark)* PnL) + TempPnL) / ((currentLine - PerformenceLogBookmark)+1);
                    
                //mean the numbers
                }
                else{
                    PnL = atof(PerfSortBuffer);
                }
                strcpy(PerfSortBuffer, "");
                NumsSorted++;
                break;
                case 1:
                strcpy(Date, PerfSortBuffer);
                 strcpy(PerfSortBuffer, "");
                 NumsSorted++;
            case 2:
                 

             if ((currentLine - PerformenceLogBookmark)>= 1)
                {
                    TempTime = atoi(PerfSortBuffer);
                    Time = (((currentLine - PerformenceLogBookmark)* Time) + TempTime) / ((currentLine - PerformenceLogBookmark)+1);
                    
                //mean the numbers
                }
                else{
                    Time = atoi(PerfSortBuffer);
                }
                 strcpy(PerfSortBuffer, "");
                 NumsSorted++;
                break;
            default:
                break;
            }
                }
            }
            else
              { NumsSorted = 0;
                break;}
            if (sizeof(line) < 1) // if line is a empty string
            {
                // submit the final result
                // IF PNL is positive, 
                if (PnL > 0)
                {
                    PerformanceRating = 0;
                }
                else   {
                    PerformanceRating = 1;
                }
                MedianWaitTime = Time;
               
if (currentLine - PerformenceLogBookmark > 3)
{
    PerformenceLogBookmark = currentLine;
    TimesChecked++; // is only alive while the app is, Resets to zero at app reset, 
}else{printf("Not enough data for performance check");}

if (TimesChecked >= 3)
{
    TimesChecked = 0;
    BadSequencesInARow++;
}
if (BadSequencesInARow != 0)
{
    TimeToWait = MedianWaitTime * (BadSequencesInARow * 10);
}

else{
    TimeToWait = MedianWaitTime;
}
                if (TimeToWait < 60000)
                {
                    TimeToWait = 60000; // 1 second // Grand Entropy adjuster comming soon In Version 5.00 // while from now, IF this shows promise, If it is horrid, But acts reliably, Itll be invested into, If it makes a good trade, But is fixably jank , Invested into.
                }
                

                char bufferPL[10];
                snprintf(bufferPL, sizeof(bufferPL),"%d",PerformenceLogBookmark); // I forgot why I need to keep this value, Oh, incase I restart the app, And dont want to have to Clear the old data only needs to read NEW data, Only data is for me personally nnot the app, Yeah. 
                SaveSetting("settings.txt", 21, bufferPL);
                fclose(txtfile);
                return;
            }  
}
}
}

}







bool DoIt = false;
bool InitiateTimer = false;

void BrokerADeal(){ // gets called NON STOP, always checkign for a trade

if (Buy == true || Sell == true)
{//ToastNotification("Thinking..!", 0);
     if (RLMODE == 0 && Buy == true)
    {//JudgePerformence();
        DoIt = true;
    } 

    if (RLMODE == 1 && Sell == true)
    {
        //TimeToWait = 0; // Should be updated to the same or similar val as it was depending on the judges evalutation, This just stops a wait from happening during the sell phase.
        DoIt = true;// THIS sends the signal, timing and all
    } 
}
else{
    return;
}

}






        
        // Mandatory 1 minute Minimum wait + TimeToWait // TIme to wait Increases NO FASTER than every 3 bad trades OVERALL, sO IF I GET 2 - 3% but 1 +10% overal we did good,
        //Waittimeishandledinthe Body,Doit activatesfirst, Oncewaittime good,Then send
        // If it goes on a winning streak it basically does nothing
        //Send Message until recieved // 
        // the Send Messenger NEeds to be Independant of this if statemnet, This is just the final decision making part, The Big weighted Wait will be here tho.
        
         //NO Minimum wait To sell, may strangely be needed but I dont think so Yeah so it doesnt insta sell, and the wait will have it ignore some short lived volatility and open the door for more long standing trades instead. 
        //Send Message until recieved check if 30 seconds have passed since the last send, IF so Send again, Increa // Once message is recieved continue to next steps,

        // Its all in the hands of the algorithm go through the data timestamped and compare it with the chart, see what Levels would be winning the most accorss that time frame,
        // maybe if market slow auto lower the NEeded amount to go for a trade? hmm
// MAIN thing left to do, The Text logger, text reader, And The Minimum wait code
//The Thirty seconds wait between sends just gives us a spread to get around and short server lag spikes Or Fickleness of the Judging algorithm

  // aNTI spam func, IF Bool is true, Buy, Dont send again until this amount of time has passed, Bool is still true and Bool of IfMessageRecieved is still False
    //IF BOOL Buy is true, and Bool IfMessageRecive is false and Time limit is up, Send again, Once sent again Increase cycle count by 1,
    // will send out the message 5 times with a UNIQUE id that is the Ticks Cycle number from the API, Buy bool stays true as long as it wants to
    //wwants too via the algorithms calculations, Hence the secondary bool, Itll keep trying to send but fail because the message was recieved,
    // Status REset upon Nor Latch Toggle from Buy to Sell,
    // Calculate good determination to switch from one to another, Simple and clean. 
    // If too many back and forths made in too short of a time span, Say 10 in 1 hr, Multiply the minimum wait time by 10, then reset the count to be from that moment on
    // from the time of the  reset forward, IF the bots record of buy and sell On its own Excution speed, The price At the moment of bool Activation
    // To the price at the time of Nor Latch switching, (we only do longs) If it is profitable OVERRIDE the Exponentional increase of minimum wait.
    // Additive percent gain, If up by even .1% let it do its thing, 
    //Now just the norlatch logic, We got all the BUy and sell Spam Logic,
    // NorLatch logic, Simple, If Buy, Buy Because thats ALL we havent done that we need to do, Also when sending out these signal Have a proceeding Code so we can ignore old info
    // Ignore old info OR correct ourselves if we have missed a couple, Which is a super big failure that should come if we have a OK connection.
    // The app regulates if its stilla  good time to buy and sell on its own, So just send me a message if the signal is bad, Somehow.
    //If buy, Buy, IF sell comes on AFter a Buy, Yeah a Chain, If you bought, we are only accepting Sells, and so on, with a minimum wait 
    //that only activates when it tries to make the switch
    //And Viola, its done.






// iF any of these variables change, OR 7 seconds have passed, Send a signal out, The full brick of all info,
// then at the website side 
// Nah, not if they change, Theres cheaper ways to find incongruency.
// if the sent Tick is Less than the latest the website just sent out,
// web side, If tick sent as a response is 2 less(doing 2 avoids us having to do precise timign calculation, its not that serious) than the latest sent, ITs atleast 1 that it missed,
//if (GlobalPTick > PrevGlobalPTick | ( ) )
//{
    /* code */
//}
//PrevGlobalPTick = 0;






// Which ever hand last became active that triggers the ability to display all active hands at the bottom scrolling, In order of last to become active, SO
// AS a hand becomes active, Its ID gets concatenized to the beginning of the string, and displayed
// SDL delay inside the X val slide loop should fix the sync problem
int main(int argc, char* argv[]) {
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); 
    SDL_Window* window = SDL_CreateWindow("Thousand Divine Palms: Enrapturing Light", 512, 512, 0| SDL_WINDOW_BORDERLESS);
    SDL_Renderer* Firstrenderer = SDL_CreateRenderer(window, NULL);

    SDL_Surface* icon = SDL_LoadBMP("images/ThousandDivinePalm.bmp");
    SDL_SetWindowIcon(window, icon);

    SDL_Surface* bgSurface = SDL_LoadBMP("images/background.bmp");

    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(Firstrenderer, bgSurface);

    SDL_Surface* btnSurface = SDL_LoadBMP("images/button.bmp");

    SDL_Texture* btnTexture = SDL_CreateTextureFromSurface(Firstrenderer, btnSurface);

    SDL_FRect buttonRect = {218, 256, 80, 38};

    bool quit = false, buttonClicked = false, switchWindow = false;


    /// ## ///   AUDIO   /// ## ///

    SDL_AudioSpec spec;
    Uint8 *audioData = NULL;
    Uint32 audioLength = 0;

    if (!SDL_LoadWAV("audio/Amor.wav", &spec, &audioData, &audioLength))
    {
    printf("Load failed: %s\n", SDL_GetError());
    }

    SDL_AudioStream *stream =
    SDL_OpenAudioDeviceStream(
    SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
    &spec,
    NULL,
    NULL);

    if (!stream)
    {
    printf("Open failed: %s\n", SDL_GetError());
    }

    SDL_ResumeAudioStreamDevice(stream);

    if (!SDL_PutAudioStreamData(stream, audioData, audioLength))
    {
    printf("Queue failed: %s\n", SDL_GetError());
    }

    

    /// ## ///   AUDIO   /// ## ///

    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)

                quit = true;
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int x = e.button.x, y = e.button.y;
                if (!buttonClicked &&
                    x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
                    y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                        SDL_ClearAudioStream(stream);
                            SDL_LoadWAV("audio/StartSound.wav", &spec, &audioData, &audioLength);
                           SDL_PutAudioStreamData(stream,
                              audioData,
                              audioLength);
                   // Any setup to input the users API link, Or use the one recorded in the text doc, displayed in the text box already, If changed, it updates the text doc, // Future feature, V3.v  
                    buttonClicked = true;
                }
            }
        }

        SDL_RenderClear(Firstrenderer);
        SDL_RenderTexture(Firstrenderer, bgTexture, NULL, NULL);
        SDL_RenderTexture(Firstrenderer, btnTexture, NULL, &buttonRect);
        SDL_RenderPresent(Firstrenderer);
//int queued_bytes = SDL_GetAudioStreamQueued(deviceId);
        if (buttonClicked) {
            switchWindow = true;
            break;
        }
        SDL_Delay(50);
    }

   // SDL_CloseAudioDevice(deviceId);
  //  SDL_free(wavBuffer);
 //   SDL_free(startBuffer);
    SDL_DestroyTexture(btnTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(Firstrenderer);
    SDL_DestroyWindow(window);

if (switchWindow) {
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
SDL_Window *win = SDL_CreateWindow("Thousand Divine Palm : EL",1442,655, 0);
SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);

//int plointX = i x gap + SCStartingX,
//75 pixels available for hand count activation chart, 9 pixel margin top and bottom
SDL_Surface *gui = SDL_LoadBMP("images/gui.bmp");
SDL_Surface *StarPloint = SDL_LoadBMP("images/PlointStar.bmp");
SDL_Surface *Text = SDL_LoadBMP("images/Text.bmp"); 
SDL_Surface* PriceChartNotifArrowSurface = SDL_LoadBMP("images/PriceChartNotifArrow.bmp");
SDL_Surface* BlueknobSurface = SDL_LoadBMP("images/BlueSlider.bmp");
SDL_Surface* RedknobSurface  = SDL_LoadBMP("images/RedSlider.bmp");
SDL_Surface* StartButtonSurface  = SDL_LoadBMP("images/StartButton.bmp");
SDL_Surface* StopButtonSurface  = SDL_LoadBMP("images/StopButton.bmp");
SDL_Surface* SaveButtonSurface  = SDL_LoadBMP("images/SaveButton.bmp");
SDL_Surface* SimulButtonSurface  = SDL_LoadBMP("images/SimulButton.bmp");  //sync because it just links the app with a data sorce, simulated or real, price, volume, Tick
SDL_Surface* NullSaveButtonSurface  = SDL_LoadBMP("images/NullSaveButton.bmp");
SDL_Surface* NullSimulButtonSurface  = SDL_LoadBMP("images/NullSimulButton.bmp");
SDL_Surface* FocusWheelZeroSurface  = SDL_LoadBMP("images/FocusNeutral.bmp");
SDL_Surface* FocusWheelOneSurface  = SDL_LoadBMP("images/FocusOne.bmp");
SDL_Surface* FocusWheelTwoSurface  = SDL_LoadBMP("images/FocusTwo.bmp");
SDL_Surface* FocusWheelThreeSurface  = SDL_LoadBMP("images/FocusThree.bmp");
SDL_Surface* FocusWheelFourSurface  = SDL_LoadBMP("images/FocusFour.bmp");
SDL_Surface* sheetSurface = SDL_LoadBMP("images/BlueFlameSprite.bmp");
SDL_Surface* BookButtonSurface = SDL_LoadBMP("images/BookButton.bmp");
SDL_Texture *guitexture = SDL_CreateTextureFromSurface(renderer, gui);
SDL_Texture *StarPlointTexture = SDL_CreateTextureFromSurface(renderer, StarPloint);
SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(renderer, Text);
SDL_Texture *PriceChartNotifArrowtexture = SDL_CreateTextureFromSurface(renderer, PriceChartNotifArrowSurface);
SDL_Texture *Blueknobtexture = SDL_CreateTextureFromSurface(renderer, BlueknobSurface);
SDL_Texture *RedknobtTexture = SDL_CreateTextureFromSurface(renderer, RedknobSurface);
SDL_Texture *StartButtonTexture = SDL_CreateTextureFromSurface(renderer, StartButtonSurface);
SDL_Texture *StopButtontexture = SDL_CreateTextureFromSurface(renderer, StopButtonSurface);
SDL_Texture *SaveButtonTexture = SDL_CreateTextureFromSurface(renderer, SaveButtonSurface);
SDL_Texture *SimulButtonTexture = SDL_CreateTextureFromSurface(renderer, SimulButtonSurface);
SDL_Texture *NullSimulButtonTexture = SDL_CreateTextureFromSurface(renderer, NullSimulButtonSurface);
SDL_Texture *NullSaveButtonTexture = SDL_CreateTextureFromSurface(renderer, NullSaveButtonSurface);
SDL_Texture *FocusZeroTexture = SDL_CreateTextureFromSurface(renderer, FocusWheelZeroSurface);
SDL_Texture *FocusOneTexture = SDL_CreateTextureFromSurface(renderer, FocusWheelOneSurface);
SDL_Texture *FocusTwoTexture = SDL_CreateTextureFromSurface(renderer, FocusWheelTwoSurface);
SDL_Texture *FocusThreeTexture = SDL_CreateTextureFromSurface(renderer, FocusWheelThreeSurface);
SDL_Texture *FocusFourTexture = SDL_CreateTextureFromSurface(renderer, FocusWheelFourSurface);
SDL_Texture *BookButtonTexture = SDL_CreateTextureFromSurface(renderer, BookButtonSurface);

SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, sheetSurface);

SDL_FRect LoopButton = {167, 9, 66, 22};
SDL_FRect SimulButton = {242, 9, 66, 22};
SDL_FRect SaveButton = {317, 9, 66, 22};
SDL_FRect FocusWheelZero = {265, 124, 21, 21};
SDL_FRect FocusWheelOne = {236, 95, 38, 38};
SDL_FRect FocusWheelTwo = {277, 95, 38, 38};
SDL_FRect FocusWheelThree = {236, 136, 38, 38};
SDL_FRect FocusWheelFour = {277, 136, 38, 38};
SDL_FRect BookButtonLoc = {1146,536, 51,68};

SDL_FRect PageTurnLeft = {0,0, 40,646};
SDL_FRect PageTurnRight = {606,0, 40,646};
SDL_FRect HandRank = {220,20, 38,38};
SDL_FRect TimeSpreadLeft = {485,337, 30,16};
SDL_FRect TimeSpreadRight = {577,337, 30,16};
SDL_FRect PriceSpreadLeft = {485,357, 30,16}; 
SDL_FRect PriceSpreadRight = {577,357, 30,16};// Finish this, all press and hold buttons, Then make some slow down, Make a seperate order,
SDL_FRect TimeMagnitudeLeft = {495,380, 30,16}; //switch all these to hold down buttons, Hold down it goes up
SDL_FRect TimeMagnitudeRight = {585,380, 30,16};
SDL_FRect HandSaveButton = {458,457, 66,26};

// ? ? ?  NETWORKING   ? ? ? //

// ? ? ?  NETWORKING   ? ? ? //

 srand(time(NULL));

SDL_Event interact;
 float mouseX, mouseY;
while (!newQuit) {

  static Uint32 lastTime = 0;
  static Uint32 LastTimeThirty = 0;
  static Uint32 TimeOfActivation = 0;
  Uint32 currentTime = SDL_GetTicks();
  SDL_FPoint mousePoint = { mouseX, mouseY };
           
 while (SDL_PollEvent(&interact)) {
            if (interact.type == SDL_EVENT_QUIT) {                                             //   THE BUG IS HERE
                newQuit = true;                                                                       //
            }                                                                                     //    SOMETHING WITH THIS WINDOWS START AND STOP PROCEDURE, I close the app, And the code starts, It gets caught up whenever this loop is called, REPLAIN IT and render some things before it
                

 

Uint32 winID = SDL_GetWindowID(win);
      if (wateranimationComplete == true)
      {
        for (int i = 0; i < 1024; i++) {
             if(interact.motion.windowID == winID){
                if (SDL_PointInRectFloat(&mousePoint, &dots[i].rect)) {
                    if (dots[i].id != lastHoveredId) {
                        if (currentTime - dots[i].lastHoverTime >= 100) {
                            dots[i].lastHoverTime = currentTime;
                            printf("Hovered Dot ID: %d\n", i+1);
                            printf("Hovered Dot Height: %d\n", dots[i].height);
                        }
                        lastHoveredId = dots[i].id;
                   }
                }
            }
        }
    }
    int x = interact.button.x;
    int y = interact.button.y;

   if (interact.type == SDL_EVENT_MOUSE_WHEEL)
    {
         if (interact.wheel.y > 0 && PriceChartSTARTINGPOINT < PriceChartMemMax)
    {
        PriceChartSTARTINGPOINT+= 5;
    }
    else if (interact.wheel.y < 0 && PriceChartSTARTINGPOINT > 0)
    {
       PriceChartSTARTINGPOINT-= 5;
    }
       // printf("Wheel: x=%f y=%f\n", interact.wheel.x, interact.wheel.y);
    }
if (interact.type == SDL_EVENT_MOUSE_WHEEL) {
  if (x >= PriceChartScrollButton.x && x <= (PriceChartScrollButton.x + PriceChartScrollButton.w) &&
    y >= PriceChartScrollButton.y && y <= (PriceChartScrollButton.y + PriceChartScrollButton.h))  {
        
  
 
} 
}

                if (interact.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {



                    if (interact.button.button == SDL_BUTTON_LEFT) {
                        SDL_FPoint mousePoint = { interact.button.x, interact.button.y };
                        if (SDL_PointInRectFloat(&mousePoint, &BlueSliderKnob)) {
                            BluedraggingSlider = true;
                        }
                        if (SDL_PointInRectFloat(&mousePoint, &RedSliderKnob)) {
                            ReddraggingSlider = true;
                        }
                    }
                } else if (interact.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (interact.button.button == SDL_BUTTON_LEFT) {
                        BluedraggingSlider = false;
                        ReddraggingSlider = false;
                    }
                } else if (interact.type == SDL_EVENT_MOUSE_MOTION) {
                    if (BluedraggingSlider) {
                        int newX = interact.motion.x;
                        newX = MAX(BluesliderStartX, MIN(newX, BluesliderStartX + BluesliderMaxRange));
                        BlueSliderKnob.x = newX;
                        // Each 2 pixels = 1 point
                        BuyPoint = (BlueSliderKnob.x - BluesliderStartX) / 2;
                        if (BuyPoint < 1){ BuyPoint = 1;}
                        char bufferBP[10];
                        snprintf(bufferBP, sizeof(bufferBP),"%d",BuyPoint);
                        SaveSetting("settings.txt", 11, bufferBP);
                        //printf("Set BuyPoint: %d%%\n", BuyPoint);
                       // SDL_Delay(50);
                    }
                    if (ReddraggingSlider) {
                        int newX = interact.motion.x;
                        newX = MAX(RedsliderStartX, MIN(newX, RedsliderStartX + RedsliderMaxRange));
                        RedSliderKnob.x = newX;
                        SellPoint = (RedSliderKnob.x - RedsliderStartX) / 2;
                        if (SellPoint < 1){ SellPoint = 1;}
                        char bufferSP[10];
                        snprintf(bufferSP, sizeof(bufferSP),"%d",SellPoint);
                        SaveSetting("settings.txt", 12, bufferSP);
                        //printf("Set SellPoint: %d%%\n", SellPoint);
                       // SDL_Delay(50);
                    }
                } 

    if (interact.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
   
 if (x >= SimulButton.x && x <= (SimulButton.x + SimulButton.w) &&
    y >= SimulButton.y && y <= (SimulButton.y + SimulButton.h))  {
        Simul = !Simul;
    }

    if (x >= LoopButton.x && x <= (LoopButton.x + LoopButton.w) &&
    y >= LoopButton.y && y <= (LoopButton.y + LoopButton.h))  {
        loopActive = !loopActive;
    }
    if (x >= SaveButton.x && x <= (SaveButton.x + SaveButton.w) &&
        y >= SaveButton.y && y <= (SaveButton.y + SaveButton.h))  {
            if (loopActive == true){
                ToastNotification("Cant Save while Trading!", 0);
            }
            else
            {
                ToastNotification("Saving Files!", 0);
            SaveFiles(filenames);
            }

    }
      
          if (x >= FocusWheelOne.x && x <= (FocusWheelOne.x + FocusWheelOne.w) &&
        y >= FocusWheelOne.y && y <= (FocusWheelOne.y + FocusWheelOne.h))  {
        FocusWheelSetting = 1;
printf("Focus %d\n", FocusWheelSetting);
SaveSetting("settings.txt",20,"1");
        }
          if (x >= FocusWheelTwo.x && x <= (FocusWheelTwo.x + FocusWheelTwo.w) &&
        y >= FocusWheelTwo.y && y <= (FocusWheelTwo.y + FocusWheelTwo.h))  {
        FocusWheelSetting = 2;
printf("Focus %d\n", FocusWheelSetting);
SaveSetting("settings.txt",20,"2");
        }
          if (x >= FocusWheelThree.x && x <= (FocusWheelThree.x + FocusWheelThree.w) &&
        y >= FocusWheelThree.y && y <= (FocusWheelThree.y + FocusWheelThree.h))  {
        FocusWheelSetting = 3;
printf("Focus %d\n", FocusWheelSetting);
SaveSetting("settings.txt",20,"3");
        }
          if (x >= FocusWheelFour.x && x <= (FocusWheelFour.x + FocusWheelFour.w) &&
        y >= FocusWheelFour.y && y <= (FocusWheelFour.y + FocusWheelFour.h))  {
        FocusWheelSetting = 4;
printf("Focus %d\n", FocusWheelSetting);
SaveSetting("settings.txt",20,"4");
        }
        if (x >= BookButtonLoc.x && x <= (BookButtonLoc.x + BookButtonLoc.w) &&
        y >= BookButtonLoc.y && y <= (BookButtonLoc.y + BookButtonLoc.h))  {
        BookOpen = !BookOpen;
        }
          if (x >= FocusWheelZero.x && x <= (FocusWheelZero.x + FocusWheelZero.w) &&
        y >= FocusWheelZero.y && y <= (FocusWheelZero.y + FocusWheelZero.h))  {
        FocusWheelSetting = 0;
printf("Focus %d\n", FocusWheelSetting);
SaveSetting("settings.txt",20,"0");
        }
 
          
      if (x >= PageTurnLeft.x && x <= (PageTurnLeft.x + PageTurnLeft.w) &&
        y >= PageTurnLeft.y && y <= (PageTurnLeft.y + PageTurnLeft.h))  {
           PageStatus = 1;
            // DONT increment here, Instead toggle a switch that increments automatically,, Then Turn ALL Increment switches off If Mouse up is detected or Mouse leaves the screen/window
        }
        
 if (x >= HandSaveButton.x && x <= (HandSaveButton.x + HandSaveButton.w) &&
        y >= HandSaveButton.y && y <= (HandSaveButton.y + HandSaveButton.h))  {
           ApplyAndSaveHTA = true;
        }

        if (x >= PageTurnRight.x && x <= (PageTurnRight.x + PageTurnRight.w) &&
        y >= PageTurnRight.y && y <= (PageTurnRight.y + PageTurnRight.h))  {
           PageStatus = 2;
        }
        

         if (x >= TimeMagnitudeLeft.x && x <= (TimeMagnitudeLeft.x + TimeMagnitudeLeft.w) &&
        y >= TimeMagnitudeLeft.y && y <= (TimeMagnitudeLeft.y + TimeMagnitudeLeft.h))  {
          TimeMagnitudeStatus = 1;
        }

        
         if (x >= TimeMagnitudeRight.x && x <= (TimeMagnitudeRight.x + TimeMagnitudeRight.w) &&
        y >= TimeMagnitudeRight.y && y <= (TimeMagnitudeRight.y + TimeMagnitudeRight.h))  {
          TimeMagnitudeStatus = 2;
        }

         if (x >= TimeSpreadRight.x && x <= (TimeSpreadRight.x + TimeSpreadRight.w) &&
        y >= TimeSpreadRight.y && y <= (TimeSpreadRight.y + TimeSpreadRight.h))  {
           TimeSpreadStatus = 2;
        }

          if (x >= TimeSpreadLeft.x && x <= (TimeSpreadLeft.x + TimeSpreadLeft.w) &&
        y >= TimeSpreadLeft.y && y <= (TimeSpreadLeft.y + TimeSpreadLeft.h))  {
        TimeSpreadStatus = 1;
        }

         if (x >= PriceSpreadLeft.x && x <= (PriceSpreadLeft.x + PriceSpreadLeft.w) &&
        y >= PriceSpreadLeft.y && y <= (PriceSpreadLeft.y + PriceSpreadLeft.h))  {
            PriceSpreadStatus = 1;
        }

         if (x >= PriceSpreadRight.x && x <= (PriceSpreadRight.x + PriceSpreadRight.w) &&
        y >= PriceSpreadRight.y && y <= (PriceSpreadRight.y + PriceSpreadRight.h))  {
      PriceSpreadStatus = 2;
        }
          if (x >= PlointCTLeft.x && x <= (PlointCTLeft.x + PlointCTLeft.w) &&
        y >= PlointCTLeft.y && y <= (PlointCTLeft.y + PlointCTLeft.h))  {
           PlointCTStatus = 1;
             
        }
          if (x >= PlointCTRight.x && x <= (PlointCTRight.x + PlointCTRight.w) &&
        y >= PlointCTRight.y && y <= (PlointCTRight.y + PlointCTRight.h))  {
           PlointCTStatus = 2;
        }

        for (int d = 1; d < Plointct; d++){
if (x >= Plointdest[d].x && x <= (Plointdest[d].x + Plointdest[d].w) &&
        y >= Plointdest[d].y && y <= (Plointdest[d].y + Plointdest[d].h))  {
            PlointMoving = d;
        }
        }
    }

if (PlointMoving != 0)
{
    int Remainder =  (589 - 301) - (Plointdest[PlointMoving].x -301); 
if (Remainder > ((Plointct - 1 - PlointMoving) * 9))
{
 Plointdest[PlointMoving].x = interact.motion.x; // works but they freeze // ITs fixed
}
else{
    Plointdest[PlointMoving].x = Plointdest[PlointMoving].x -1;
}
Plointdest[PlointMoving].y = interact.motion.y;
 if (Plointdest[PlointMoving].x >= 590){
    Plointdest[PlointMoving].x = 589;
 }
 //{Plointdest[PlointMoving].x = Plointdest[PlointMoving -1].x +3;} // PERFECT system, Modable min, Dont do the max.

}

     if (interact.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        PlointCTStatus = 0;
        PageStatus = 0;
        TimeSpreadStatus = 0;
        TimeMagnitudeStatus = 0;
        PriceSpreadStatus = 0;
        PlointMoving = 0;
        HoldTimer = 500;
        CycleCT = 0;
        //Start a Counter /timer when pressed down, Reset that here too, The more it goes up the more rapid the values are queried if they should increase, Cycle rate scaler,
     } // aslo destroy surface of the images, Dont forget to do that, Memory issue,
 
 SDL_GetMouseState(&mouseX, &mouseY);
}
if (PlointCTStatus != 0){ //Press and hold button method I designed
     CycleCT++;
    HoldTimer -= 1;
    if (HoldTimer/100 <CycleCT){
    CycleCT = 0;
if (Plointct > 2) {if (PlointCTStatus == 1) {Plointct--;}}
if (Plointct < 10) {if (PlointCTStatus == 2) {Plointct++;}}
    }
}

if (PageStatus != 0){ //Press and hold button method I designed
    CycleCT++;
    HoldTimer -= 1;
    if (HoldTimer/100 <CycleCT)
    { CycleCT = 0;
if (Page > 0) {if (PageStatus == 1) {Page--;}}
if (Page < HandTotal) {if (PageStatus == 2) {Page++;}}
    }
    

}

if (TimeMagnitudeStatus != 0){ //Press and hold button method I designed
if (TimeMagnitudef >= 0) {if (TimeMagnitudeStatus == 1) {TimeMagnitudef-=5;}}
if (TimeMagnitudef <= 10000) {if (TimeMagnitudeStatus == 2) {TimeMagnitudef+=5;}}
}

if (TimeSpreadStatus!= 0){ //Press and hold button method I designed
if (TimeSpreadf > 5) {if (TimeSpreadStatus == 1) {TimeSpreadf--;}}
if (TimeSpreadf < 95) {if (TimeSpreadStatus == 2) {TimeSpreadf++;}}
}

if (PriceSpreadStatus != 0){ //Press and hold button method I designed
if (PriceSpreadf > 5) {if (PriceSpreadStatus == 1) {PriceSpreadf--;}}
if (PriceSpreadf < 95) {if (PriceSpreadStatus == 2) {PriceSpreadf++;}}
}
//printf("Page Number %d\n", Page);




        // Set window icon from file
       // icon = SDL_LoadBMP("images/ThousandDivinePalm.ico"); // Might NEED to be bmp
       // if (icon) {
      //      SDL_SetWindowIcon(win, icon);
      //      SDL_DestroySurface(icon);
       // }
//Implement old text from images method 

       
SDL_RenderTexture(renderer, guitexture, NULL, NULL);
if (runonce < 1)
{
loopActive == false;


for (int d = 0; d < 4; d++)
{
    RuleOfThreeParts* init = RuleOfThreeBinder[d];
    init->NextTickToCheck = 0;
    init->Magnitude = 1;
    for (int w = 0; w < d; w++)
    {
         init->Magnitude *= 10; 
        
    }
    // printf("R of 3 Mag%d\n",init->Magnitude );
 
   for (int h = 0; h <3; h++)
    {    
         init->CountToThree[h].Found= false;
         init->CountToThree[h].price = 0;
         init->CountToThree[h].Tick= 0;
    }
}


for (int f = 0; f < (sizeof(*RelativeLocationA)/2); f++)
{
    HandTypePlointArch* Seed = RelativeLocationA[f];
    FetchHandLog(f);
    Seed->StartingTick = 0;
    Seed->NextTicktoCheck = 0;

   Seed->Size = 10;
   Seed->NextTicktoCheck = 0;
   Seed->StartingTick = 0;
   Seed->TimeSpread = 5;
   Seed->PriceSpread = 5;
   Seed->LastPlointFound = 0;
   Seed->TimeMagnitude = 0;
   for (int h = 0; h < 9; h++)
    {
        Seed->Times[h] = 0;
    }
}

for (int qq = 0; qq < PriceChartMemMax; qq++)
{
    PriceChart[qq].location.x = 395; // Could make a Smooth transition function, Sends off the price its supposed to be, The value itself,  Increment it in that direction once every tick // Would need a dynamic Global static value tho, mandatory Direct Memory control Unless we Do it in the body of the app, and use a Sequencer
            PriceChart[qq].location.w = 1 ; 
            PriceChart[qq].location.y = 518;
            PriceChart[qq].location.y = 40;
}

for (int f = 0; f < HSRPlointcount; f++)
{
 BuySRateChart[f].height = 0;
 SellSRateChart[f].height = 0;
}


SlideMax = SlidingTextXVal / SlideRate;
createDumpFiles();
BlueSliderKnob.x = (RPercentSelected * 2) + BluesliderStartX;
RedSliderKnob.x = (LPercentSelected * 2) + RedsliderStartX;
runonce = runonce + 1;
 CreateFogMap(); 
 HTTP_StartServer();

 CreateThread(
    NULL,              // security attributes
    0,                 // stack size (0 = default)
    HTTPThread,        // function to run
    NULL,              // argument
    0,                 // creation flags
    NULL               // thread id (optional)
);
printf("INITIALIZED\n\n");}

 HTTP_StartServer();
            
            for (size_t i = 0; i < 1024; i++) 
             {
                SDL_SetRenderDrawColor(renderer, 107, 107, 107, 115);  // make this back 44
                SDL_RenderFillRect(renderer, &dots[i].rect);
                SDL_FRect inner = {dots[i].rect.x + (dots[i].rect.w / 2), dots[i].rect.y + (dots[i].rect.h / 2), 1, 1 };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
                SDL_RenderFillRect(renderer, &inner);
                    if (dots[i].height > 0 | dots[i].RH > 0 ){
                        if (dots[i].height > dots[i].RH)
                        {
                            dots[i].RH++;
                        }
                        if (dots[i].height < dots[i].RH)
                        {
                            dots[i].RH--;
                        }
                        
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 8);
                    SDL_FRect Top = {dots[i].rect.x,dots[i].rect.y - dots[i].RH,3,3};
                    SDL_FRect innerT = {dots[i].rect.x + (dots[i].rect.w / 2), (dots[i].rect.y - dots[i].RH)+(dots[i].rect.h/2),1,1};
                    SDL_FRect line = {dots[i].rect.x +1,dots[i].rect.y+1,1,-1 - dots[i].RH};
                    SDL_RenderFillRect(renderer, &line);
                     SDL_RenderFillRect(renderer, &Top);
                    SDL_RenderFillRect(renderer, &innerT);
                }
            }
       






// ? ? ?  NETWORKING   ? ? ? //

/* // Currently it just silently tries to connect until it does, Will I ever need or want a switch>
if (ConnectionActivated)
{  

}*/


    //HTTP_Process();


// ? ? ?  NETWORKING   ? ? ? //







if (wateranimationComplete == false)
{
    
   WaterClockSpeed++; 
   //printf("Queueing Animation %d%%\n", WaterClockSpeed);
if (WaterClockSpeed == 100)
{
    
int WaterAnimation [256];

if (wateranimationframe == 5)
{
    int PatternFour[256]={
0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0, 
0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
,0,0,0,0,0,0,0,0
};
printf("Clearing Animation\n");
memcpy(WaterAnimation, PatternFour, sizeof(WaterAnimation));
wateranimationComplete = true;
}  

if (wateranimationframe == 4){ 
    int PatternFive[256]={ // no need to rename them, just for human convenience
70,78,70,55,40,25,15,8,8,15,25,40,55,70,78,70,
78,72,60,45,30,18,10,6,6,10,18,30,45,60,72,78,
70,60,48,34,22,13,7,4,4,7,13,22,34,48,60,70,
55,45,34,25,16,9,5,3,3,5,9,16,25,34,45,55,
40,30,22,16,10,6,3,2,2,3,6,10,16,22,30,40,
25,18,13,9,6,4,2,1,1,2,4,6,9,13,18,25,
15,10,7,5,3,2,1,1,1,1,2,3,5,7,10,15,
8,6,4,3,2,1,1,1,1,1,1,2,3,4,6,8,
8,6,4,3,2,1,1,1,1,1,1,2,3,4,6,8,
15,10,7,5,3,2,1,1,1,1,2,3,5,7,10,15,
25,18,13,9,6,4,2,1,1,2,4,6,9,13,18,25,
40,30,22,16,10,6,3,2,2,3,6,10,16,22,30,40,
55,45,34,25,16,9,5,3,3,5,9,16,25,34,45,55,
70,60,48,34,22,13,7,4,4,7,13,22,34,48,60,70,
78,72,60,45,30,18,10,6,6,10,18,30,45,60,72,78,
70,78,70,55,40,25,15,8,8,15,25,40,55,70,78,70
  
};  
wateranimationframe = wateranimationframe + 1;
//printf("Frame Animation %d\n", wateranimationframe);
memcpy(WaterAnimation, PatternFive, sizeof(WaterAnimation));
}   

if (wateranimationframe == 3){ 
    int PatternFour[256]={
90,88,75,60,42,28,18,12,12,18,28,42,60,75,88,90,
88,78,65,50,34,22,14,9,9,14,22,34,50,65,78,88,
75,65,52,38,25,16,10,6,6,10,16,25,38,52,65,75,
60,50,38,28,18,11,7,4,4,7,11,18,28,38,50,60,
42,34,25,18,12,8,5,3,3,5,8,12,18,25,34,42,
28,22,16,11,8,5,3,2,2,3,5,8,11,16,22,28,
18,14,10,7,5,3,2,1,1,2,3,5,7,10,14,18,
12,9,6,4,3,2,1,1,1,1,2,3,4,6,9,12,
12,9,6,4,3,2,1,1,1,1,2,3,4,6,9,12,
18,14,10,7,5,3,2,1,1,2,3,5,7,10,14,18,
28,22,16,11,8,5,3,2,2,3,5,8,11,16,22,28,
42,34,25,18,12,8,5,3,3,5,8,12,18,25,34,42,
60,50,38,28,18,11,7,4,4,7,11,18,28,38,50,60,
75,65,52,38,25,16,10,6,6,10,16,25,38,52,65,75,
88,78,65,50,34,22,14,9,9,14,22,34,50,65,78,88,
90,88,75,60,42,28,18,12,12,18,28,42,60,75,88,90


};
wateranimationframe += 1;
//printf("Frame Animation %d\n", wateranimationframe);
memcpy(WaterAnimation, PatternFour, sizeof(WaterAnimation));
}   

if (wateranimationframe == 2){ 
    int PatternThree[256]={
100,95,80,55,35,22,14,9,9,14,22,35,55,80,95,100,
95,85,65,45,28,17,10,6,6,10,17,28,45,65,85,95,
80,65,55,38,24,14,8,5,5,8,14,24,38,55,65,80,
55,45,38,30,18,10,6,4,4,6,10,18,30,38,45,55,
35,28,24,18,13,7,4,3,3,4,7,13,18,24,28,35,
22,17,14,10,7,5,3,2,2,3,5,7,10,14,17,22,
14,10,8,6,4,3,2,1,1,2,3,4,6,8,10,14,
9,6,5,4,3,2,1,1,1,1,2,3,4,5,6,9,
9,6,5,4,3,2,1,1,1,1,2,3,4,5,6,9,
14,10,8,6,4,3,2,1,1,2,3,4,6,8,10,14,
22,17,14,10,7,5,3,2,2,3,5,7,10,14,17,22,
35,28,24,18,13,7,4,3,3,4,7,13,18,24,28,35,
55,45,38,30,18,10,6,4,4,6,10,18,30,38,45,55,
80,65,55,38,24,14,8,5,5,8,14,24,38,55,65,80,
95,85,65,45,28,17,10,6,6,10,17,28,45,65,85,95,
100,95,80,55,35,22,14,9,9,14,22,35,55,80,95,100
};
wateranimationframe += 1;
//printf("Frame Animation %d\n", wateranimationframe);
memcpy(WaterAnimation, PatternThree, sizeof(WaterAnimation));
}   

if (wateranimationframe == 1){ 
    int PatternTwo[256]={
100,82,55,32,18,10,6,4,4,6,10,18,32,55,82,100,
82,68,45,25,14,8,5,3,3,5,8,14,25,45,68,82,
55,45,35,20,12,7,4,2,2,4,7,12,20,35,45,55,
32,25,20,15,9,5,3,2,2,3,5,9,15,20,25,32,
18,14,12,9,7,4,2,1,1,2,4,7,9,12,14,18,
10,8,7,5,4,3,2,1,1,2,3,4,5,7,8,10,
6,5,4,3,2,2,1,1,1,1,2,2,3,4,5,6,
4,3,2,2,1,1,1,1,1,1,1,1,2,2,3,4,
4,3,2,2,1,1,1,1,1,1,1,1,2,2,3,4,
6,5,4,3,2,2,1,1,1,1,2,2,3,4,5,6,
10,8,7,5,4,3,2,1,1,2,3,4,5,7,8,10,
18,14,12,9,7,4,2,1,1,2,4,7,9,12,14,18,
32,25,20,15,9,5,3,2,2,3,5,9,15,20,25,32,
55,45,35,20,12,7,4,2,2,4,7,12,20,35,45,55,
82,68,45,25,14,8,5,3,3,5,8,14,25,45,68,82,
100,82,55,32,18,10,6,4,4,6,10,18,32,55,82,100


};
wateranimationframe += 1;;
//printf("Frame Animation %d\n", wateranimationframe);
memcpy(WaterAnimation, PatternTwo, sizeof(WaterAnimation));
}      

if (wateranimationframe == 0)
{
int PatternOne[256]={
100,70,40,20,10,5,3,2,2,3,5,10,20,40,70,100,
70,55,30,15,8,4,2,1,1,2,4,8,15,30,55,70,
40,30,20,10,5,3,1,1,1,1,3,5,10,20,30,40,
20,15,10,6,3,2,1,1,1,1,2,3,6,10,15,20,
10,8,5,3,2,1,1,1,1,1,1,2,3,5,8,10,
5,4,3,2,1,1,1,1,1,1,1,1,2,3,4,5,
3,2,1,1,1,1,1,1,1,1,1,1,1,1,2,3,
2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
3,2,1,1,1,1,1,1,1,1,1,1,1,1,2,3,
5,4,3,2,1,1,1,1,1,1,1,1,2,3,4,5,
10,8,5,3,2,1,1,1,1,1,1,2,3,5,8,10,
20,15,10,6,3,2,1,1,1,1,2,3,6,10,15,20,
40,30,20,10,5,3,1,1,1,1,3,5,10,20,30,40,
70,55,30,15,8,4,2,1,1,2,4,8,15,30,55,70,
100,70,40,20,10,5,3,2,2,3,5,10,20,40,70,100

};
wateranimationframe  += 1;;
//printf("Frame Animation %d\n", wateranimationframe);
memcpy(WaterAnimation, PatternOne, sizeof(WaterAnimation));
}

          
int CurrentDot = 0;
int timing = 0;

for (int p= 0; p < 16; p++) {
 
for (int k= 0; k < 16; k++) {

    dots[CurrentDot + k].height = WaterAnimation[timing];

  int MirroredValX = 31 - k + (32 * p); // Perfect
  int MirroredValZag = 1023 - k - (32 * p); // Perfect
  int MirroredValY = 992 + k - (32 * p); // Perfect
  dots[MirroredValX].height = WaterAnimation[timing];
  dots[MirroredValZag].height = WaterAnimation[timing];
  dots[MirroredValY].height = WaterAnimation[timing];
timing++;
}
CurrentDot += 32 ;
}
 

WaterClockSpeed = 0;
}
}
// The closer the coordinate num is to the border the less it needs to add to itself to get the coord of its transformation site.
// so we are gonna create a sliding scale, Its gonna be th first 8 numbers are assigned the first 8 numbers in the animation.

 

        //   //  // SETTING ASSIGNMENT AREA //    //    //

        

     Setting SetS = {"Empty"}; // Load up user changeable settings here,
     Setting SetA = {"Empty"}; // This is the part that takes them from
     Setting SetB = {"Empty"}; // The Text file and puts them into global
     Setting SetC = {"Empty"}; // Variables.
     Setting SetD = {"Empty"};
     Setting SetTrueNeutral = {"Empty"}; //Changes nothing, so sum is zero, everywhere always
     Setting SetHuman = {"Empty"};
     Setting SetBomb = {"Empty"};
     Setting SetBabel = {"Empty"};
     Setting SetIndustry = {"Empty"};
     Setting RPSelected = {"Empty"};
     Setting LPSelected = {"Empty"};
     Setting SetFocusWheel = {"Empty"};
     Setting SetPerformenceLogBookmark = {"Empty"};
// * * * Settings Area * * * // 
     Setting *settings[] = {&SetS, &SetA, &SetB, &SetC, &SetD,&SetTrueNeutral,&SetHuman,&SetBomb,&SetBabel,&SetIndustry,&RPSelected,&LPSelected,&SetFocusWheel,&SetPerformenceLogBookmark};

    FILE *pF = fopen("settings.txt", "r");
    char setting[255];
    if(pF == NULL)
    {
       perror("fopen");
    }
    for (int i = 0; i < sizeof(settings)/sizeof(settings[0]); i++) { 
            fgets(setting, sizeof(setting), pF);
            strcpy(settings[i]->set, setting);
            //printf("Loading Settings\r ");
        }
        fclose(pF); 

        // AT THIS POINT in the code, all settings are already pulled from the settings text file.
        // In the MARKED line below all Settings to used variable conversions and assignments are to be done in the same area,

        WeightClass SRank = {"S Class",1}; //100
        WeightClass ARank = {"A Class",2}; //80
        WeightClass BRank = {"B Class",3}; //60
        WeightClass CRank = {"C Class",4}; //40
        WeightClass DRank = {"D Class",5}; //20

        Virtue TrueNeutral = {100,false};
        Virtue Human = {100,false};
        Virtue Bomb = {100,false};
        Virtue Babel = {100,false};
        Virtue Industry = {100,false};


        SRank.Weight = atoi(SetS.set);
        ARank.Weight = atoi(SetA.set);
        BRank.Weight = atoi(SetB.set);
        CRank.Weight = atoi(SetC.set);
        DRank.Weight = atoi(SetD.set);
        RPercentSelected = atoi(RPSelected.set);
        LPercentSelected = atoi(LPSelected.set);

        PerformenceLogBookmark = atoi(SetPerformenceLogBookmark.set);
       FocusWheelSetting = atoi(SetFocusWheel.set); // RE instate this if you want it to be in the settings menu Thats for the UPDATE V2

        BoostDefault.Babel.Boost = atoi(SetBabel.set);
        BoostDefault.Bomb.Boost = atoi(SetBomb.set);
        BoostDefault.Human.Boost = atoi(SetHuman.set);
        BoostDefault.Industry.Boost = atoi(SetIndustry.set);
        BoostDefault.TrueNeutral.Boost = atoi(SetTrueNeutral.set);
        
        
        
        //   //  // SETTING ASSIGNMENT AREA //    //    //
//SaveFiles(filenames);


            //  // *** // *** // *** //  //
        //  // THE // THOUSAND // PALMS //  //
           //  // *** // *** // *** //  //


Hand I;
I.IDNUM = 1;
strcpy(I.ID, "I");
strcpy(I.Name, "Rule Of Three!");
strcpy(I.Descriptor, "The First Hand*Not Sure what Algorithm*Im putting here yet!");
I.PageType = 1;
I.Weight = SRank;
I.ActivationRate = 00.00; // make a for loop to initialize these all as 00.00
I.Activation = false;
I.virt.Industry.Active = false;
I.virt.Babel.Active = false;
I.virt.Bomb.Active = false;
I.virt.Human.Active = false;
CopyBoosts(&I.virt, &BoostDefault); // gotta copy this over to all hands

Hand II;
II.IDNUM = 2;
strcpy(II.ID, "II");
strcpy(II.Name, "Cup N Handle Classic!");
strcpy(II.Descriptor, "A legitimate Trading*Technique. And at the same*Time Majorly Fraudulent.*Once king. it may never be*king again.!");
II.PageType = 2;
II.Weight = SRank;
II.ActivationRate = 00.00;
II.Activation = false;
II.virt.Industry.Active = false;
II.virt.Babel.Active = false;
II.virt.Bomb.Active = false;
II.virt.Human.Active = false;
CopyBoosts(&II.virt, &BoostDefault); // gotta copy this over to all hands

Hand III;
III.IDNUM = 3;
strcpy(III.ID, "III");
strcpy(III.Name, "Sell Sword!");
strcpy(III.Descriptor, "RANDOM!");
III.PageType = 2;
III.Weight = SRank;
III.ActivationRate = 00.00;
III.Activation = false;
III.virt.Industry.Active = false;
III.virt.Babel.Active = false;
III.virt.Bomb.Active = false;
III.virt.Human.Active = false;
CopyBoosts(&III.virt, &BoostDefault); // gotta copy this over to all hands

Hand IV;
IV.IDNUM = 4;
strcpy(IV.ID, "IV");
strcpy(IV.Name, "Rule Of Thiry!");
strcpy(IV.Descriptor, "RANDOM!");
IV.PageType = 1;
IV.Weight = SRank;
IV.ActivationRate = 00.00;
IV.Activation = false;
IV.virt.Industry.Active = false;
IV.virt.Babel.Active = false;
IV.virt.Bomb.Active = false;
IV.virt.Human.Active = false;
CopyBoosts(&IV.virt, &BoostDefault); // gotta copy this over to all hands

Hand V;
V.IDNUM = 5;
strcpy(V.ID, "V");
strcpy(V.Name, "Rule Of Three Hundred!");
strcpy(V.Descriptor, "RANDOM!");
V.PageType = 1;
V.Weight = SRank;
V.ActivationRate = 00.00;
V.Activation = false;
V.virt.Industry.Active = false;
V.virt.Babel.Active = false;
V.virt.Bomb.Active = false;
V.virt.Human.Active = false;
CopyBoosts(&V.virt, &BoostDefault); // gotta copy this over to all hands

Hand VI;
VI.IDNUM = 6;
strcpy(VI.ID, "VI");
strcpy(VI.Name, "Rule of Three Thousand!");
strcpy(VI.Descriptor, "RANDOM!");
VI.PageType = 1;
VI.Weight = SRank;
VI.ActivationRate = 00.00;
VI.Activation = false;
VI.virt.Industry.Active = false;
VI.virt.Babel.Active = false;
VI.virt.Bomb.Active = false;
VI.virt.Human.Active = false;
CopyBoosts(&VI.virt, &BoostDefault); // gotta copy this over to all hands

Hand VII;
VII.IDNUM = 7;
strcpy(VII.ID, "VII");
strcpy(VII.Name, "NA!");
strcpy(VII.Descriptor, "RANDOM!");
VII.PageType = 2;
VII.Weight = SRank;
VII.ActivationRate = 00.00;
VII.Activation = false;
VII.virt.Industry.Active = false;
VII.virt.Babel.Active = false;
VII.virt.Bomb.Active = false;
VII.virt.Human.Active = false;
CopyBoosts(&VII.virt, &BoostDefault); // gotta copy this over to all hands

Hand VIII;
VIII.IDNUM = 8;
strcpy(VIII.ID, "VIII");
strcpy(VIII.Name, "Leaping Frog!");
strcpy(VIII.Descriptor, "RANDOM!");
VIII.PageType = 1;
VIII.Weight = SRank;
VIII.ActivationRate = 00.00;
VIII.Activation = false;
VIII.virt.Industry.Active = false;
VIII.virt.Babel.Active = false;
VIII.virt.Bomb.Active = false;
VIII.virt.Human.Active = false;
CopyBoosts(&VIII.virt, &BoostDefault); // gotta copy this over to all hands

Hand IX;
IX.IDNUM = 9;
strcpy(IX.ID, "IX");
strcpy(IX.Name, "Ditsy Daisies!");
strcpy(IX.Descriptor, "RANDOM!");
IX.PageType = 1;
IX.Weight = SRank;
IX.ActivationRate = 00.00;
IX.Activation = false;
IX.virt.Industry.Active = false;                // This need be declared only once, Re arrange this if you can, You can, App testing due we know, Love you Programmer guy
IX.virt.Babel.Active = false;
IX.virt.Bomb.Active = false;
IX.virt.Human.Active = false;
CopyBoosts(&IX.virt, &BoostDefault); // gotta copy this over to all hands

Hand X;
X.IDNUM = 10;
strcpy(X.ID, "X");
strcpy(X.Name, "NA!");
strcpy(X.Descriptor, "RANDOM!");
X.PageType = 1;
X.Weight = SRank;
X.ActivationRate = 00.00;
X.Activation = false;
X.virt.Industry.Active = false;
X.virt.Babel.Active = false;
X.virt.Bomb.Active = false;
X.virt.Human.Active = false;
CopyBoosts(&X.virt, &BoostDefault); // gotta copy this over to all hands

Hand XI;
XI.IDNUM = 11;
strcpy(XI.ID, "XI");
strcpy(XI.Name, "NA!");
strcpy(XI.Descriptor, "RANDOM!");
XI.PageType = 1;
XI.Weight = SRank;
XI.ActivationRate = 00.00;
XI.Activation = false;
XI.virt.Industry.Active = false;
XI.virt.Babel.Active = false;
XI.virt.Bomb.Active = false;
XI.virt.Human.Active = false;
CopyBoosts(&XI.virt, &BoostDefault); // gotta copy this over to all hands

Hand XII;
XII.IDNUM = 12;
strcpy(XII.ID, "XII");
strcpy(XII.Name, "Champions Glory!");
strcpy(XII.Descriptor, "RANDOM d!");
XII.PageType = 1;
XII.Weight = SRank; // Assigning the "S Class"
XII.ActivationRate = 00.00;
XII.Activation = false;
CopyBoosts(&XII.virt, &BoostDefault); // gotta copy this over to all hands
XII.virt.Industry.Active = false;
XII.virt.Babel.Active = false;
XII.virt.Bomb.Active = false;
XII.virt.Human.Active = false;

Hand XIII;
XIII.IDNUM = 13;
strcpy(XIII.ID, "XIII");
strcpy(XIII.Name, "No Name!");
strcpy(XIII.Descriptor, "RANDOM description Hasnt*a Meaning really im Just*spouting words to test!");
XIII.PageType = 1;
XIII.Weight = SRank;
XIII.ActivationRate = 00.00;
XIII.Activation = false;
CopyBoosts(&XIII.virt, &BoostDefault);
XIII.virt.Industry.Active = false;
XIII.virt.Babel.Active = false;
XIII.virt.Bomb.Active = false;
XIII.virt.Human.Active = false;

Hand XIV;
XIV.IDNUM = 14;
strcpy(XIV.ID, "XIV");
strcpy(XIV.Name, "No Name!");
strcpy(XIV.Descriptor, "RANDOM!");
XIV.PageType = 1;
XIV.Weight = SRank;
XIV.ActivationRate = 00.00;
XIV.Activation = false;
CopyBoosts(&XIV.virt, &BoostDefault);
XIV.virt.Industry.Active = false;
XIV.virt.Babel.Active = false;
XIV.virt.Bomb.Active = false;
XIV.virt.Human.Active = false;

Hand XV;
XV.IDNUM = 15;
strcpy(XV.ID, "XV");
strcpy(XV.Name, "No Name!");
strcpy(XV.Descriptor, "RANDOM!");
XV.PageType = 1;
XV.Weight = SRank;
XV.ActivationRate = 00.00;
XV.Activation = false;
CopyBoosts(&XV.virt, &BoostDefault);
XV.virt.Industry.Active = false;
XV.virt.Babel.Active = false;
XV.virt.Bomb.Active = false;
XV.virt.Human.Active = false;

Hand XVI;
XVI.IDNUM = 16;
strcpy(XVI.ID, "XVI");
strcpy(XVI.Name, "No Name!");
strcpy(XVI.Descriptor, "RANDOM!");
XVI.PageType = 1;
XVI.Weight = SRank;
XVI.ActivationRate = 00.00;
XVI.Activation = false;
CopyBoosts(&XVI.virt, &BoostDefault);
XVI.virt.Industry.Active = false;
XVI.virt.Babel.Active = false;
XVI.virt.Bomb.Active = false;
XVI.virt.Human.Active = false;

Hand XVII;
XVII.IDNUM = 17;
strcpy(XVII.ID, "XVII");
strcpy(XVII.Name, "No Name!");
strcpy(XVII.Descriptor, "RANDOM!");
XVII.PageType = 1;
XVII.Weight = SRank;
XVII.ActivationRate = 00.00;
XVII.Activation = false;
CopyBoosts(&XVII.virt, &BoostDefault);
XVII.virt.Industry.Active = false;
XVII.virt.Babel.Active = false;
XVII.virt.Bomb.Active = false;
XVII.virt.Human.Active = false;
 
Hand XVIII; 
XVIII.IDNUM = 18;
strcpy(XVIII.ID, "XVIII"); 
strcpy(XVIII.Name, "No Name!"); 
strcpy(XVIII.Descriptor, "RANDOM!");
XVIII.PageType = 2;
XVIII.Weight = SRank; 
XVIII.ActivationRate = 00.00; 
XVIII.Activation = false; 
CopyBoosts(&XVIII.virt, &BoostDefault);
XVIII.virt.Industry.Active = false;
XVIII.virt.Babel.Active = false;
XVIII.virt.Bomb.Active = false;
XVIII.virt.Human.Active = false;

Hand XIX;
XIX.IDNUM = 19;
strcpy(XIX.ID, "XIX");
strcpy(XIX.Name, "No Name!");
strcpy(XIX.Descriptor, "RANDOM!");
XIX.PageType = 1;
XIX.Weight = SRank;
XIX.ActivationRate = 00.00;
XIX.Activation = false;
CopyBoosts(&XIX.virt, &BoostDefault);
XIX.virt.Industry.Active = false;
XIX.virt.Babel.Active = false;
XIX.virt.Bomb.Active = false;
XIX.virt.Human.Active = false;

Hand XX;
XX.IDNUM = 20;
strcpy(XX.ID, "XX");
strcpy(XX.Name, "No Name!");
strcpy(XX.Descriptor, "RANDOM!");
XX.PageType = 1;
XX.Weight = SRank;
XX.ActivationRate = 00.00;
XX.Activation = false;
CopyBoosts(&XX.virt, &BoostDefault);
XX.virt.Industry.Active = false;
XX.virt.Babel.Active = false;
XX.virt.Bomb.Active = false;
XX.virt.Human.Active = false;

Hand XXI;
XXI.IDNUM = 21;
strcpy(XXI.ID, "XXI");
strcpy(XXI.Name, "No Name!");
strcpy(XXI.Descriptor, "RANDOM!");
XXI.PageType = 1;
XXI.Weight = SRank;
XXI.ActivationRate = 00.00;
XXI.Activation = false;
CopyBoosts(&XXI.virt, &BoostDefault);
XXI.virt.Industry.Active = false;
XXI.virt.Babel.Active = false;
XXI.virt.Bomb.Active = false;
XXI.virt.Human.Active = false;

Hand XXII;
XXII.IDNUM = 22;
strcpy(XXII.ID, "XXII");
strcpy(XXII.Name, "No Name!");
strcpy(XXII.Descriptor, "RANDOM!");
XXII.PageType = 1;
XXII.Weight = SRank;
XXII.ActivationRate = 00.00;
XXII.Activation = false;
CopyBoosts(&XXII.virt, &BoostDefault);
XXII.virt.Industry.Active = false;
XXII.virt.Babel.Active = false;
XXII.virt.Bomb.Active = false;
XXII.virt.Human.Active = false;

Hand XXIII;
XXIII.IDNUM = 23;
strcpy(XXIII.ID, "XXIII");
strcpy(XXIII.Name, "No Name!");
strcpy(XXIII.Descriptor, "RANDOM!");
XXIII.PageType = 1;
XXIII.Weight = SRank;
XXIII.ActivationRate = 00.00;
XXIII.Activation = false;
CopyBoosts(&XXIII.virt, &BoostDefault);
XXIII.virt.Industry.Active = false;
XXIII.virt.Babel.Active = false;
XXIII.virt.Bomb.Active = false;
XXIII.virt.Human.Active = false;

Hand XXIV;
XXIV.IDNUM = 24;
strcpy(XXIV.ID, "XXIV");
strcpy(XXIV.Name, "No Name!");
strcpy(XXIV.Descriptor, "RANDOM!");
XXIV.PageType = 1;
XXIV.Weight = SRank;
XXIV.ActivationRate = 00.00;
XXIV.Activation = false;
CopyBoosts(&XXIV.virt, &BoostDefault);
XXIV.virt.Industry.Active = false;
XXIV.virt.Babel.Active = false;
XXIV.virt.Bomb.Active = false;
XXIV.virt.Human.Active = false;


Hand* Righthands[12] = {&II, &IV, &VI, &VIII, &X,  &XII, &XIV, &XVI,&XVIII,&XX,&XXII, &XXIV};
Hand* Lefthands[12] = {&I, &III, &V, &VII, &IX, &XI,&XIII, &XV,  &XVII,  &XIX, &XXI,  &XXIII}; // order these as evens and odds, Right hands evens, Left hands Odds, Left hands first

            //  // *** // *** // *** //  //
        //  // THE // THOUSAND // PALMS //  //
            //  // *** // *** // *** //  //
// Here we will call the animation Functions, Wave of the price charts and the water drop function of the diamond hand



        // Define fixed animation points
        SDL_FPoint animationPoints[21] = {
            {1039, 420}, {1010, 398}, {985, 368}, {973, 333},
            {963, 295}, {955, 260}, {958, 222}, {970, 180},
            {1005, 113}, {1052, 76}, {1264, 76},  {1313, 113},
            {1343, 180}, {1351, 222}, {1361, 260}, {1364, 295},
            {1353, 333}, {1339, 368}, {1313, 398}, {1288, 420}, {1160, 220}  
        }; 
SDL_FRect FireDestRect;
SDL_FRect FireSrcRect;

float placeholder  = RLMODE ? .2 * ((100/ (sizeof(Righthands)/4)) * RTotalActive): .2 * ((100/ (sizeof(Lefthands)/4)) * LTotalActive);


int AnimationPointCT = (int)(placeholder * 2.33);
if (AnimationPointCT > 21){ AnimationPointCT  = 21;};
          for (int i = 0; i < AnimationPointCT; i++) {

            if ( prevplaceholder != AnimationPointCT )
            { prevplaceholder = AnimationPointCT;
                        SDL_ClearAudioStream(stream);
                SDL_free(audioData);
          SDL_LoadWAV("audio/Flame.wav", &spec, &audioData, &audioLength);

              SDL_ResumeAudioStreamDevice(stream);
                           SDL_PutAudioStreamData(stream,
                              audioData,
                              audioLength);
                              
            }
            
             FireSrcRect.y = FrameHeight;
             FireSrcRect.x = FrameWidth * currentFrame;
             FireSrcRect.h = FrameHeight;
             FireSrcRect.w = FrameWidth;
             
             FireDestRect.y = animationPoints[i].y;
             FireDestRect.x = animationPoints[i].x;
             FireDestRect.h = FrameHeight;
             FireDestRect.w = FrameWidth;
                 SDL_RenderTexture(renderer, spriteTexture, &FireSrcRect, &FireDestRect); // Doesnt render for some reason, Need sleep
            }
           
 currentFrame++;
 if (currentFrame == FrameCount)
 {
    currentFrame = 0;
 }

     
if (NotifDelay == 70 + strlen(ToastNotif.Text)){
    NotifStart = false;
    NotifDelay = 0;
}


if (NotifStart == true)
{
NotifDelay++;
//SDL_SetRenderDrawColor(renderer,200, 200, 250, 8);
//SDL_RenderFillRect(renderer, &TextBackground);
for (int d = 0; d < strlen(ToastNotif.Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &ToastNotif.srcrect[d], &ToastNotif.destrect[d]);
}
}


strcpy(GUITextBox[0].Text, "$BTC!");// "YOU ARE A KING*WE ARE APPLYING TO JANE STREET.*ALL OTHERS CANT COMPETE!");
TypeTextToScreen(&GUITextBox[0], 1, 860, 10);

for (int d = 0; d < strlen(GUITextBox[0].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[0].srcrect[d], &GUITextBox[0].destrect[d]);
}

char Tbuffer[100];
char Tickbuffer[100] = "Tick ";
snprintf(Tbuffer, sizeof(Tbuffer), "%d", AlgoTick); // NOT the price tick, The Computational Tick of the Algo Processing, With HArcPloint, Send off Latesttick not Global
strcat(Tickbuffer, Tbuffer);
strcat(Tickbuffer, "!");
strcpy(GUITextBox[1].Text, Tickbuffer);
TypeTextToScreen(&GUITextBox[1], 1, 165, 37);

for (int d = 0; d < strlen(GUITextBox[1].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[1].srcrect[d], &GUITextBox[1].destrect[d]);
}
      
   
SDL_SetRenderDrawColor(renderer,50, 50, 255, 255);
ActivationPercentageGauge[0].location.x = 22 ;
ActivationPercentageGauge[0].location.y = 241 ;
ActivationPercentageGauge[0].location.w = 51 ;
float PercenttoBuy = (100 / BuyPnt) * RScore; // equalS the percent 
ActivationPercentageGauge[0].location.h = -(PercenttoBuy  * 2);
SDL_RenderFillRect(renderer, &ActivationPercentageGauge[0].location);

SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
ActivationPercentageGauge[1].location.x = 87 ;
ActivationPercentageGauge[1].location.y = 241 ;
ActivationPercentageGauge[1].location.w = 51 ;
float PercenttoSell = (100 / SellPnt) * LScore;
ActivationPercentageGauge[1].location.h = -(PercenttoSell * 2); 
SDL_RenderFillRect(renderer, &ActivationPercentageGauge[1].location);
            
SDL_FRect SlideCoords;
SlideCoords.x = SlidingTextXVal;
SlidingText(0);
SlideCoords.w = -( 30 * SlideCounter);
SlideCoords.h = 24;
SlideCoords.y = 626;


char SlidingTextBuffer[100];
//RLMODE ? strcpy(SlidingTextBuffer, LpreviousActiveIDs) : strcpy(SlidingTextBuffer, RpreviousActiveIDs);
strcpy(SlidingTextBuffer, ScrollingNames);
strcat(SlidingTextBuffer, "!");
strcpy(GUITextBox[2].Text,SlidingTextBuffer);
TypeTextToScreen(&GUITextBox[2], 1, SlideCoords.x, 629); //The Sliding text function has its own Char but its not fully implemented beacuse Im not sure how I want to use it, SO Relevant data, Not yet tailored in anyway,
strcpy(SlidingTextBuffer, "");
for (int d = 0; d < strlen(GUITextBox[2].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[2].srcrect[d], &GUITextBox[2].destrect[d]);
}


char Pricebuffer[100];
char Pbuffer[100];
snprintf(Pbuffer, sizeof(Pbuffer), "%.2lf", GlobalPrice);
strcpy(Pricebuffer, "Price$");
strcat(Pricebuffer, Pbuffer);
strcat(Pricebuffer, "!");
strcpy(GUITextBox[3].Text, Pricebuffer);
TypeTextToScreen(&GUITextBox[3], 1, 400, 10);

for (int d = 0; d < strlen(GUITextBox[3].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[3].srcrect[d], &GUITextBox[3].destrect[d]);
}
         
        
char Volumebuffer[100] = "Volume $";
char Vbuffer[100];
if (Simul == true)
{
snprintf(Vbuffer, sizeof(Vbuffer), "%.3lf Billion", GlobalVolume);
}
else {
snprintf(Vbuffer, sizeof(Vbuffer), "%.2lf", GlobalVolume);
}
strcat(Volumebuffer,Vbuffer);
strcat(Volumebuffer, "!");
strcpy(GUITextBox[4].Text, Volumebuffer);
TypeTextToScreen(&GUITextBox[4], 1, 400, 40);

for (int d = 0; d < strlen(GUITextBox[4].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[4].srcrect[d], &GUITextBox[4].destrect[d]);
}


char Climaxbuffer[100] = "Climax %"; 
char Cbuffer[100];
snprintf(Cbuffer, sizeof(Cbuffer), "%.2f", Climax);
strcat(Climaxbuffer,Cbuffer);
strcat(Climaxbuffer, "!");
strcpy(GUITextBox[5].Text, Climaxbuffer);
TypeTextToScreen(&GUITextBox[5], 1, 165, 62);

for (int d = 0; d < strlen(GUITextBox[5].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[5].srcrect[d], &GUITextBox[5].destrect[d]);
}

char HandVSbuffer[100]; 
char HVersusbuffer[100];
snprintf(HVersusbuffer, sizeof(HVersusbuffer),"%d", RTotalActive); 
strcat(HandVSbuffer, HVersusbuffer);
strcat(HandVSbuffer, "             ");
snprintf(HVersusbuffer, sizeof(HVersusbuffer),"%d", LTotalActive); 
strcat(HandVSbuffer,HVersusbuffer);
strcat(HandVSbuffer, "!");
strcpy(GUITextBox[6].Text, HandVSbuffer); 
strcpy(HandVSbuffer, "");  
TypeTextToScreen(&GUITextBox[6], 1, 212, 205);

for (int d = 0; d < strlen(GUITextBox[6].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[6].srcrect[d], &GUITextBox[6].destrect[d]);
}


char Hanbuffer[100]; // I realize Now I could just use one buffer and clean it before every new use, But uhh, Next project Ill do that, Id likely learn something else before completing this one and be stuck here forever, Perfecting whats already done.
snprintf(Hanbuffer, sizeof(Hanbuffer),"%d             %lu", RTotalActive + LTotalActive, (sizeof(Lefthands) + sizeof(Righthands)) /8); 
strcat(Hanbuffer, "!");
strcpy(GUITextBox[7].Text, Hanbuffer); 
strcpy(Hanbuffer, "");  // String cat was just repeating endlessly, Forgot to rinse the buffer, Is good now.
TypeTextToScreen(&GUITextBox[7], 1, 212, 255);// Its causing a memory leak im not sure why

for (int d = 0; d < strlen(GUITextBox[7].Text); d++)
{
  SDL_RenderTexture(renderer, TextTexture, &GUITextBox[7].srcrect[d], &GUITextBox[7].destrect[d]);
}



               for (int i = 0; i < HSRPlointcount;  i++) {
            DisplaySRateChart[i].location.x = 19 + HSRPlointGap * i;
            DisplaySRateChart[i].location.w = 13 ;
            DisplaySRateChart[i].location.h = 13 ;
           DisplaySRateChart[i].location.y = 600;
            DisplaySRateChart[i].location.y -= RLMODE ? (SellSRateChart[i].height * .68) : (BuySRateChart[i].height * .68) ;
            if ( DisplaySRateChart[i].location.y < 532 )
            {
                 DisplaySRateChart[i].location.y = 532;
            }
            SDL_RenderTexture(renderer, StarPlointTexture, NULL, & DisplaySRateChart[i].location);
          
        }
     
            for (int j = (PriceChartMemMax - PriceChartSTARTINGPOINT) - PriceChartDisplayRange  ; j <  PriceChartMemMax - PriceChartSTARTINGPOINT;  j++) { 
           
            PriceChart[j].location.x = 910 + PricePlointGap * (j - (PriceChartMemMax - PriceChartSTARTINGPOINT)); // Could make a Smooth transition function, Sends off the price its supposed to be, The value itself,  Increment it in that direction once every tick // Would need a dynamic Global static value tho, mandatory Direct Memory control Unless we Do it in the body of the app, and use a Sequencer
            PriceChart[j].location.w = 1 ; 
            PriceChart[j].location.y = 518;  // Add a Smooth transition On this // and In a later version the ability to scrolll, Starting Line as a distance From the latest, Scroll back and U are 1 line from the latest, So you view all the latest data but 1 line behind, ! line as in How many pixels are u from base line
            SDL_SetRenderDrawColor(renderer,255,255, 255, 185); // That can be subtracted from to equal A full line of data, So if the line draw is 2 pixels wide, with 1 seperator pixel, Ever 3 pixels is 1 line away, so that chart displayed is Latest Data - lets say -189, So show the data stored, Up until -189,and if the total width of the viewport is 400
            SDL_RenderFillRect(renderer, &PriceChart[j].location); // Then thats Its to display FROM LATEST POSITION -(189 +400) and onward
             
             }   
             
             if (PriceChartSTARTINGPOINT > 0)
             { flicker++;
                if ( flicker % flickerRate == 0)
                {
                 Toggle = !Toggle;
                  flicker = 0;
                }
                if (Toggle == true)
                {
                     SDL_RenderTexture(renderer, PriceChartNotifArrowtexture, NULL, &PriceChartScrollsTATus);
                }
             }
             
            

            SDL_RenderTexture(renderer, Blueknobtexture, NULL, &BlueSliderKnob);
            SDL_RenderTexture(renderer, RedknobtTexture, NULL, &RedSliderKnob);

          switch (FocusWheelSetting)
          {
        case 0:
            SDL_RenderTexture(renderer, FocusZeroTexture, NULL, &FocusWheelZero);
            break;
        case 1:
            SDL_RenderTexture(renderer, FocusOneTexture, NULL, &FocusWheelOne);
            break;
        case 2:
             SDL_RenderTexture(renderer, FocusTwoTexture, NULL, &FocusWheelTwo);
            break;
        case 3:
            SDL_RenderTexture(renderer, FocusThreeTexture, NULL, &FocusWheelThree);
            break;
        case 4:
            SDL_RenderTexture(renderer, FocusFourTexture, NULL, &FocusWheelFour);
            break;
          
          default:
            break;
          }


            if (loopActive) {
                SDL_RenderTexture(renderer, StopButtontexture, NULL, &LoopButton);
                SDL_RenderTexture(renderer, NullSaveButtonTexture, NULL, &SaveButton);
            }
            else { 
                SDL_RenderTexture(renderer, StartButtonTexture, NULL, &LoopButton);
                SDL_RenderTexture(renderer, SaveButtonTexture, NULL, &SaveButton);} 
               
                if (Simul == true)
                {
                   SDL_RenderTexture(renderer, NullSimulButtonTexture, NULL, &SimulButton);
                }
                else{
                    SDL_RenderTexture(renderer, SimulButtonTexture, NULL, &SimulButton);
                }
    


    ///BOOK//







if (Page != 0)
{
 
if (Page % 2 == 0)
{
PageState = Righthands[(Page/2) - 1]->PageType;
strcpy(DescText.Text,Righthands[Page/2 -1]->Descriptor);
strcpy(HandName.Text,Righthands[Page/2 -1]->Name);

    switch (Righthands[5]->Weight.Name[0]) // should be the letter and NOT the null terminator, If strange that may be why, Confident its correct.
    {
    case 'S':
    RanktoRender = 0;
    break;

    case 'A':
    RanktoRender = 1;
    break;
        case 'B':
    RanktoRender = 2;
    break;
        case 'C':
    RanktoRender = 3;
    break;
        case 'D':
    RanktoRender = 4;
    break;
    default:
    break;
    }
} 


if ((Page % 2) > 0){
    if (Page <= HandTotal)
    {
   
    
 PageState = Lefthands[((Page - 1) /2)]->PageType;
strcpy(DescText.Text,Lefthands[((Page - 1) /2)]->Descriptor); // The FOg map can be hovered over in the book, Not through it, But even while they are not overlayed, fix
strcpy(HandName.Text,Lefthands[((Page - 1) /2)]->Name);

    switch (Lefthands[((Page - 1) /2)]->Weight.Name[0]) // should be the letter and NOT the null terminator, If strange that may be why, Confident its correct.
    {
    case 'S':
    RanktoRender = 0;
    break;
    case 'A':
    RanktoRender = 1;
    break;
        case 'B': 
    RanktoRender = 2;
    break;
        case 'C':
    RanktoRender = 3;
    break;
        case 'D':
    RanktoRender = 4;
    break;
    default:
    break;
    } //printf("Rank %d\n", RanktoRender);
}
}
}
//message PageNumber;
//message HandName;
//message DescText;
//message TrackRecord;


// If Page state is ZERO that could be a homepage, It could be, I could make it so, Move em all up one, to its true sum   
 //message HandTypeArcsettings ORGANIZE IT, theres a better way, My method is right, Just decide what gets defined in the Right or LEft if statements above vs here, kinda duplicating;

    //Define the unique text layout and buttons layout
    //in the settings file OH MY FRICK, clutter the file or complexify its complexity, darn this data has to be pulled from a test file, In the typical Typetext to screen formate, a raw string passed into the fucntion as are all others, 
    // THE STATS for the hand for example would look like 0*0*0! for its efficacy # of time activated and etc, Short and to the point. 
    //and the Rank of the hand wil just be a Image at the top beside the name and below the Page number, The Name of the hand is stored in the code, so is the description, 
    // So lets do the ground work now and bring it home tmrw.
    //Need a Stats Message struct, a name one and one additional for type B pages that would go in the if Statement, and lastly one for the page Number, So FIVE total (page number, Name, Desc, Stats, type B settings text ),  conditional. 
    //Type A just has the right side of the book be blank












if (BookOpen == true && InitOnce == 0)
{
    InitOnce =1;
BOOK = SDL_CreateWindow("Hand Book", 646, 555, 0);
Bookrenderer = SDL_CreateRenderer(BOOK, NULL);

//SDL_SetRenderDrawColor(Bookrenderer,150, 100, 200, 255);
//SDL_RenderFillRect(Bookrenderer, &PageTurnLeft);
//SDL_SetRenderDrawColor(Bookrenderer,200, 100, 150, 255);
//SDL_RenderFillRect(Bookrenderer, &PageTurnRight);
BookText = SDL_LoadBMP("images/BookText.bmp");
bookSurface = SDL_LoadBMP("images/Book.bmp");
BookCoverSurface = SDL_LoadBMP("images/CoverPage.bmp");
BookBasicSurface = SDL_LoadBMP("images/BookBasic.bmp");
PlointSurface = SDL_LoadBMP("images/ArcPloints.bmp");
HRankS = SDL_LoadBMP("images/Gold.bmp");
HRankA = SDL_LoadBMP("images/Silver.bmp");
HRankB = SDL_LoadBMP("images/Royal.bmp");
HRankC = SDL_LoadBMP("images/Red.bmp");
HRankD = SDL_LoadBMP("images/Blue.bmp");
BookTexture = SDL_CreateTextureFromSurface(Bookrenderer, bookSurface);
BookTextTexture = SDL_CreateTextureFromSurface(Bookrenderer, BookText);
BookCoverTexture = SDL_CreateTextureFromSurface(Bookrenderer, BookCoverSurface);
BookBasicTexture = SDL_CreateTextureFromSurface(Bookrenderer, BookBasicSurface);
PlointTexture = SDL_CreateTextureFromSurface(Bookrenderer, PlointSurface);
HRankSTexture = SDL_CreateTextureFromSurface(Bookrenderer, HRankS);
HRankATexture = SDL_CreateTextureFromSurface(Bookrenderer, HRankA);
HRankBTexture = SDL_CreateTextureFromSurface(Bookrenderer, HRankB);
HRankCTexture = SDL_CreateTextureFromSurface(Bookrenderer, HRankC);
HRankDTexture = SDL_CreateTextureFromSurface(Bookrenderer, HRankD);
}

if (BookOpen) // Strange, Some parts seem dplicated but was needed to completely containerize this element, Think of it like a Airlock
{
SDL_RenderTexture(renderer, BookButtonTexture, NULL, &BookButtonLoc);
if (Page == 0)
{
    SDL_RenderTexture(Bookrenderer, BookCoverTexture, NULL, NULL);
}
else if (PageState == 1){
    SDL_RenderTexture(Bookrenderer, BookBasicTexture, NULL, NULL);
}

else if (PageState == 2)// Else Render the base for the Page state, yeah, We can adapt this as needed, Right now we only have 3 types, The intro, The plain(not implemented) and tHE arc map
{
SDL_RenderTexture(Bookrenderer, BookTexture, NULL, NULL);

char HandTypeArcsettingsbuffer[100];
strcpy(HandTypeArcsettings.Text, "   ");
snprintf(HandTypeArcsettingsbuffer, sizeof(HandTypeArcsettingsbuffer),"%d************%.2f*%.2f*  %.0f!", Plointct, TimeSpreadf, PriceSpreadf, TimeMagnitudef); 
strcat(HandTypeArcsettings.Text, HandTypeArcsettingsbuffer);
strcpy(HandTypeArcsettingsbuffer, "");
TypeTextToScreen(&HandTypeArcsettings, 1, 520, 70);
for (int d = 0; d < strlen(HandTypeArcsettings.Text); d++)
{SDL_RenderTexture(Bookrenderer, BookTextTexture, &HandTypeArcsettings.srcrect[d], &HandTypeArcsettings.destrect[d]); }

for (int d = 0; d < Plointct; d++) // 0 here will be a input variable // 6/14/2026, dont remember what I meant by input variable, Must be scrap OR already done given how my brain works, Most likely
{ 
    Plointsrc.x = 0 +(10 * d);
     
/*if (Plointdest[d].y < 108){Plointdest[d].y = 109;}
if (Plointdest[d].y > 302){Plointdest[d].y = 301;}
if (Plointdest[d].x > 590){Plointdest[d].x = 589;}  //SO 589 - 301if REmainer < (Ploint count - Currentploint Number) * 9, current ploint cant move past that number
if (Plointdest[d].x < 300){Plointdest[d].x = 301;}*/ // Buggy< Conflicts with the DOnt let it cross the other ones before it feature.
//SO 589 - 301if REmainer < (Ploint count - Currentploint Number) * 9, current ploint cant move past that number 
/*int Remainder =  (589 - 301) - (Plointdest[d].x -301); 
if (Remainder > ((Plointct - d) * 9))
{
    
}*/

//Just make it so NO one prior can move ahead of the one in front of its place -9 aka PurelyVisualPrettiness
// SO instead of moving them up it just stops moving, But i like the sweep feature so calc it to let it stop ahead,
// Cant move past if the the remainder of the space is equal too or would be less than the ACTIVE proceeding Ploints width. SO if REmainer < (Ploint count - Currentploint Number) * 9, current ploint cant move past that number, So it just pushs
// them all up against the wwall Butt to butt 
if (d > 0)
{
if (Plointdest[d].x <= Plointdest[d-1].x){Plointdest[d].x = Plointdest[d-1].x + PurelyVisualPrettiness;}
}




//Plointdest[d].x += 10 *d; // fix this, It defaults to zero, not the norm,Sterilize it , Render them seperate of all other numvers the try again

//Plointdest[d].y = 10 *d;
//printf("Ploint X %.2f, Ploint Y %.2f",Plointdest[d].x ,Plointdest[d].y);
    SDL_RenderTexture(Bookrenderer, PlointTexture, &Plointsrc, &Plointdest[d]); // Just make a array of page numbers beside that correlate to the proper struct, So Find 2 in the array, Then that say placement is the accurate place in the main array of all structs
                                                                                // So now page = struct location with this label array It wont be a function. But all In house down here, then whats applied, Then saved via a void function
}}

if (CurrentPage != Page)
{

 for (int i = 0; i < (sizeof(RelativeLocationB) /4); i++)
{
    if (RelativeLocationB[i] == Page){ // GLitched at Page 10, Likely a memory bug, havent found the source

FetchHandLog(i);

    }

     // MAKES the page show the current saved info
  
 // Page Reset Only Needed for Page Types with Modifiable data.
}

CurrentPage = Page;
}



if (ApplyAndSaveHTA == true)
{
  for (int i = 0; i < 10; i++)//10, just the number of  Hands in the log for that type // Theres only 1 at the moment
  {
    if (RelativeLocationB[i] == Page)
    {
        //Work the struct
    HandTypePlointArch* Selected = RelativeLocationA[i];

/////////////////////////////////////////////////////


  for (int b = 0; b < Selected->Size; b++)
   {
     Selected->PriceArr[b].HigherLower = 0;
     Selected->PriceArr[b].Tick = 0;
     Selected->PriceArr[b].Price = 0;
     Selected->PriceArr[b].TrueRelativity = 0;
     Selected->PriceArr[b].PlointFound = 0;
   }






    Selected->Size = Plointct;
    Selected->TimeSpread = TimeSpreadf;
    Selected->PriceSpread = PriceSpreadf;
    Selected->TimeMagnitude = TimeMagnitudef;
for (size_t y = 1; y < 10; y++) // I should be able to let it process the start too, It should all equal zero,, But I wont. Skipping it entirely is cleaner.
{
//The first point is the start, its vals are all ZEro, Its shouldnt be being read, Its a known stable position

    if (Plointdest[y].y >= Plointdest[y-1].y )
    {
       Selected->PriceArr[y].HigherLower = 0; // zero should mean Higher
       Selected->PriceArr[y].TrueRelativity = (Plointdest[y-1].y - Plointdest[y].y); // 394 is the baseline, thats just were it appears on screen, No need to sanitize these sums, the visual relativity is true.
       //printf("TR %f\n",(Plointdest[y].y - Plointdest[y-1].y));
    }
    
        if (Plointdest[y].y <= Plointdest[y-1].y) // One of them has to accept equal to avoid errors, Now my code has innate favoritism Darn it. unavoidable
    {
       Selected->PriceArr[y].HigherLower = 1; // One should mean Lower
       Selected->PriceArr[y].TrueRelativity = (Plointdest[y].y - Plointdest[y-1].y); // 394 is the baseline, thats just were it appears on screen, No need to sanitize these sums, the visual relativity is true.
       //printf("TR %f\n",(Plointdest[y-1].y - Plointdest[y].y));
    }
    //printf("HL %d", Selected->PriceArr[y].HigherLower);
    Selected->Times[y-1] = (Plointdest[y].x - Plointdest[y-1].x) - PurelyVisualPrettiness; // So everypixel is 1 x TimeMaginitude, but thall be done in the calculation phase
// ALSO with the time mag, IF number larger than, Divide the time table in seconds not just be 60 for secs, But 60 again for hours, and  if its more than a day in seconds, By 24 also, 3 if statements in sequence, E
}
ApplyAndSaveHTA = false; // A button makes this true
SavetoHandLog(i); // takes the already updated struct and puts it to the text file
printf("Saved Hand %d\n", i);
    break;
    }
  } // May still not be done right, Either a GLOBAL error Causing problems, OR what, No known source, Putting negative numbers in teh save file shouldnt happen
}


//SavetoHandLog(Page);
//SDL_SetRenderDrawColor(Bookrenderer,105,200, 205, 185);
//SDL_RenderFillRect(Bookrenderer, &PlointCTLeft);
//SDL_RenderFillRect(Bookrenderer, &TimeSpreadLeft);
//SDL_RenderFillRect(Bookrenderer, &PriceSpreadLeft);
//SDL_RenderFillRect(Bookrenderer, &TimeMagnitudeLeft);
//SDL_SetRenderDrawColor(Bookrenderer,50,205, 105, 185);
//SDL_RenderFillRect(Bookrenderer, &PlointCTRight);
//SDL_RenderFillRect(Bookrenderer, &TimeSpreadRight);
//SDL_RenderFillRect(Bookrenderer, &PriceSpreadRight);
//SDL_RenderFillRect(Bookrenderer, &HandSaveButton);



if (Page != 0)
{
char PageNumberbuffer[100];
snprintf(PageNumberbuffer, sizeof(PageNumberbuffer),"%d", Page); 
strcpy(PageNumber.Text, "");
strcat(PageNumber.Text, PageNumberbuffer); // leaves a stray dot if you go up past 2 digits then go back down, very strange will fix later
strcat(PageNumber.Text, ".!");
strcpy(PageNumberbuffer, ""); 
TypeTextToScreen(&PageNumber, 1, 280, 5);
for (int d = 0; d < strlen(PageNumber.Text); d++)
{SDL_RenderTexture(Bookrenderer, BookTextTexture, &PageNumber.srcrect[d], &PageNumber.destrect[d]);}


TypeTextToScreen(&HandName, 1, 45, 54);
for (int d = 0; d < strlen(HandName.Text); d++)
{SDL_RenderTexture(Bookrenderer, BookTextTexture, &HandName.srcrect[d], &HandName.destrect[d]);}

TypeTextToScreen(&DescText, 1, 26, 110);
for (int d = 0; d < strlen(DescText.Text); d++)
{SDL_RenderTexture(Bookrenderer, BookTextTexture, &DescText.srcrect[d], &DescText.destrect[d]);}

// Add the Rank BAdge on here too
char TrackRecordbuffer[100];
strcpy(TrackRecord.Text, "");
snprintf(TrackRecordbuffer, sizeof(TrackRecordbuffer),"%d**%d**%d!", Page, Page, Page); 
strcat(TrackRecord.Text, TrackRecordbuffer);
strcpy(TrackRecordbuffer, ""); 
TypeTextToScreen(&TrackRecord, 1, 245, 400);
for (int d = 0; d < strlen(TrackRecord.Text); d++)
{SDL_RenderTexture(Bookrenderer, BookTextTexture, &TrackRecord.srcrect[d], &TrackRecord.destrect[d]); }

if (RanktoRender == 0){SDL_RenderTexture(Bookrenderer, HRankSTexture, &RankSrc, &RankLoc);}
if (RanktoRender == 1){SDL_RenderTexture(Bookrenderer, HRankATexture, &RankSrc, &RankLoc);}
if (RanktoRender == 2){SDL_RenderTexture(Bookrenderer, HRankBTexture, &RankSrc, &RankLoc);}
if (RanktoRender == 3){SDL_RenderTexture(Bookrenderer, HRankCTexture, &RankSrc, &RankLoc);}
if (RanktoRender == 4){SDL_RenderTexture(Bookrenderer, HRankDTexture, &RankSrc, &RankLoc);}

}



SDL_RenderPresent(Bookrenderer);  
}
     
    if (BookOpen == false)
    {
    InitOnce = 0;
    SDL_DestroyWindow(BOOK);
    BOOK = NULL;
    Page=0;
    }
    




    if (DoIt == true)
    {

   
        if (InitiateTimer == false)
        { // IF String recieved(and sent back to us as confirmation) == Current To be sent, Set all vals to 0, and set message as recieved.
           TimeOfActivation = currentTime; //cant afford to corrupt The Time towait val, so it remains seperate
           InitiateTimer = true;
                char TransmitBuf[100] = "";
        snprintf(TransmitBuf, sizeof(TransmitBuf),"Transmission in %dms!",TimeToWait);
ToastNotification(TransmitBuf,  TimeToWait);
strcpy(TransmitBuf,"");
        }
        
        

          if (currentTime >= TimeOfActivation + TimeToWait )// Whatever is calculated using Performence judger logic, thats how long the wait is, Its outlined in text how it is to function YW <3
        {
// Do the Wait timer code here, 
// hand the Waittime Code here, // seperate of the 30 second cycle
//ToastNotification("Transmission in Time 2!", 0);
          if (currentTime >= LastTimeThirty + 30000){// 30,000 is supposed to be 30 seconds
   
           ToastNotification("TRANSMITTING!", 0);
            LastTimeThirty = currentTime;
            TransmissionCT++;
            
             //printf("Preparing the Data\n"); // Bogus claim but sounds positive to the user
if (Simul == false)
{
    if (RLMODE == 0)
    {
        BoughtPrice = GlobalPrice;
        if (IsMessageRecieved == 0)
        {
            // Send the Buy Signal once, Code thats in the BODY handles the repeat and its timing
        // QueueMessage("Buy");
        msgBuySignal = 1;
        }
    }
// THis is only to make trades, Another hotlink will be needed TO connect to the site, and keep both tethered,
// will transmit Price, Tick, Volume, to the app
// app will send CLimax for both Buy and sell,  RLMODE and Tick Back to the site, as a minimum, They share Tick as a Form of handshake to ensure they stay in sync, Notify me if its out of sync, Depending on hwo its ut of sync ill make a solution,
 if (RLMODE == 1)
    {
        SoldPrice = GlobalPrice;
        if (IsMessageRecieved == 0)
        {
            // Send the Sell Signal once, Code thats in the BODY handles the repeat and its timing
            msgSellSignal = 1;
        }
    }

    if (IsMessageRecieved == 1)
    {

    msgBuySignal = 0;
// Never SHould both be triggered at one time. Because of the RLMODe switch, If that happens I Got the order flipped around somewhere, 0 is Buy, 1 is Sell
    msgSellSignal = 0;
         SDL_ClearAudioStream(stream);
SDL_free(audioData);
audioData = NULL;
          SDL_LoadWAV("audio/OrderFilled.wav", &spec, &audioData, &audioLength);

              SDL_ResumeAudioStreamDevice(stream);
                           SDL_PutAudioStreamData(stream,
                              audioData,
                              audioLength);
        RLMODE = !RLMODE;

        IsMessageRecieved = 0;
        if (BoughtPrice != 0 && SoldPrice != 0)
        {
            // Log Both to the text file PerformenceLog + the time and TimeTowait Val
        // - - - ## WORK STILL TO BE DONE, THE FILE LOGGING ## - - - //
        // Style:  PercentChange_TimeStamp_TimeToWait // just those 3 things

        
float PercentChange = 0;
        if (BoughtPrice >= SoldPrice)
        {
           PercentChange = -((BoughtPrice / 100) * (BoughtPrice - SoldPrice));
        }
        if (BoughtPrice < SoldPrice)
        {
            PercentChange = ((SoldPrice / 100) * (SoldPrice - BoughtPrice)); // hasnt been tested but no reason it shouldnt work.
        }

char PerformenceBuffer[100] = "";
char PerformenceSheetFormat[100] = "";

time_t now = time(NULL);
struct tm *t = localtime(&now);
char timestamp[64];
strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", t);
snprintf(PerformenceBuffer, sizeof(PerformenceBuffer),"%.3f %s %d ", PercentChange, timestamp, TimeToWait);
strcat(PerformenceSheetFormat,PerformenceBuffer);
strcpy(PerformenceBuffer,"");
TradeCount++; // Unused atm
appendToFile("PerformenceLog.txt",PerformenceSheetFormat); // just adds it to whatever free slot is left, We can keep the val of read values thus far and never have to empty the text file
        BoughtPrice = 0; 
        SoldPrice = 0;
        DoIt = false;
        TransmissionCT = 0;
        InitiateTimer = false;
        }
        

    }
          
            if (TransmissionCT == 5) // tries to send it 5 times, if all attempts fail, check if Buy is still true, then try again,
            {
             //Stop transmitting and Check if Buy is still True RESET
             //set Buy and sell to false
             Buy = false;
             Sell = false;
             TransmissionCT = 0;
             DoIt = false; // will make it question if buy is still active, If the trade is still viable before it repeats, Bypassing the wait time Until the message is recieve, No timeouts, 
             // merely resetting Initiate timer would reset the timer, just a reminder incase i come back to this long after for whatever reason.
            }
}

if (Simul == true)
{
   RLMODE = !RLMODE;
   DoIt = false;
   Buy = false;
    Sell = false;
      InitiateTimer = false;
     SDL_ClearAudioStream(stream);
SDL_free(audioData);
audioData = NULL;
          SDL_LoadWAV("audio/OrderFilled.wav", &spec, &audioData, &audioLength);

              SDL_ResumeAudioStreamDevice(stream);
                           SDL_PutAudioStreamData(stream,
                              audioData,
                              audioLength);

    if (BoughtPrice != 0 && SoldPrice != 0)
        {
            // Log Both to the text file PerformenceLog + the time and TimeTowait Val
        // - - - ## WORK STILL TO BE DONE, THE FILE LOGGING ## - - - //
        // Style:  PercentChange_TimeStamp_TimeToWait // just those 3 things

        
float PercentChange = 0;
        if (BoughtPrice >= SoldPrice)
        {
           PercentChange = -((BoughtPrice / 100) * (BoughtPrice - SoldPrice));
        }
        if (BoughtPrice < SoldPrice)
        {
            PercentChange = ((SoldPrice / 100) * (SoldPrice - BoughtPrice)); // hasnt been tested but no reason it shouldnt work.
        }

char PerformenceBuffer[100] = "";
char PerformenceSheetFormat[100] = "";

time_t now = time(NULL);
struct tm *t = localtime(&now);
char timestamp[64];
strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", t);
snprintf(PerformenceBuffer, sizeof(PerformenceBuffer),"%.3f %s %d ", PercentChange, timestamp, TimeToWait);
strcat(PerformenceSheetFormat,PerformenceBuffer);
strcpy(PerformenceBuffer,"");
TradeCount++; // Unused atm
appendToFile("PerformenceLog.txt",PerformenceSheetFormat); // just adds it to whatever free slot is left, We can keep the val of read values thus far and never have to empty the text file
        BoughtPrice = 0; 
        SoldPrice = 0;
      
      
        } 
}


       
       }
    }
          
}
    



//HANDARCTYPE is has its data stored in the order it is called, No new organization system, THE ULTIMATE simple method


                // Algorithm LOOP //
        
if (Simul == true)
{
   if (currentTime - lastTime >= 111) {  // Just delete thi whol ebracketed segment when your done testing
SimulatePriceChange(&GlobalPrice, &GlobalVolume);
 UpdatePriceChart(GlobalPrice);
         char BufferGPrice[10] = "";
          snprintf(BufferGPrice, sizeof(BufferGPrice),"%lf",GlobalPrice);// Will have to do for Volume also later, But not Right now. Manually clear for now, Because testing phase, But auto Clear at startup o rshutdown
         appendToFile("PriceLog.txt", BufferGPrice);
         strcpy(BufferGPrice, "");
LatestTick ++;
lastTime = currentTime;
}
}

UpdatePriceChart(0); // PURELY to update the chart more often, visual effect, self sorts more often

 if (loopActive == true && AlgoTick < LatestTick) { // Just for back and forth check purposes, So it updates when a new arrives, No sooner, and no other time, GlobalPtick is useless beyond that, 
            //Just Latest, And Algo, The Latest we have and the the Part the ALgorithms are currently Caught up to 
 //basic Number Others are Logistical Truthes directly coordinated with the price data coming in,
  AlgoTick++;

  if ( AlgoTick > prevAlgoTick + 333) 
  {
   UpdateSRChart(Climax); //Updates ever 333 ticks. 3 data points every 1000 ticks,
   prevAlgoTick = AlgoTick;
  }
  

Climax = RLMODE ?   PercenttoSell : PercenttoBuy ;
BrokerADeal(); // always called always looking for a trade when its getting data.
 //printf("Calculating..\r");
//printf("Calculating..\n");
// THE PRICE USED FOR THE ALGORITHMS //

double ALGOPrice = GetPriceAtTick(AlgoTick); // Price CAtchup mechanism, Set a Proper Shutdown procedure, Log In the file Cut while live trading, Then auto prompt The user to put in the tick it left off at to insta catchup wheere it left off, Or do a AUTO check, SO click 
//printf("Algo Price $%lf\n", ALGOPrice); // DO you want to catch up? Number input box is greyed out, Auto check box is checked, have to uncheckmark it to put in a number, Auto runs the whole pricelog TO see where it left off OR uses a stored val to Collect all that data then 
// Yup, Save all the incoming Price data to a text file using the function we already have for that,
//Append,
//then get it from their so we can start trading at our own leisure, 
// THE PRICE USED FOR THE ALGORITHMS //

//??????????????????????????????????????????????????????????????//
//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//

                        // Algorithms //

//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//
//??????????????????????????????????????????????????????????????//





// so for any algorithms here they are to be fed "AlgoPrice" and "AlgoTick" only, except for strange and Unique use cases.
HandTypePloint( &II, &Two);
HandTypePloint( &III, &Three);
HandTypePloint( &VII, &Seven);
HandTypePloint( &XVIII, &EightTeen); 
//Empty LOL, Private info


RuleOfThree( &I,&One ); // Mag 1
RuleOfThree( &IV,&Four); // Mag 10
RuleOfThree( &V,&Five); // Mag 100
RuleOfThree( &VI,&Six); // Mag 1000





//??????????????????????????????????????????????????????????????//
//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//

                        // Algorithms //

//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//''//
//??????????????????????????????????????????????????????????????//




  //***//***//***//***//***//***//***//***//***//***//***// 
//  // THE // THOUSAND // PALMS // PROCESSING // CODE //  //
  //***//***//***//***//***//***//***//***//***//***//***//


RTotalActive = 0; // whether a hand deactivates is up to it, Some only need be found once and for all until reset.
// use the same Hand struct, But Make a Left hand for loop for sell methods // The expirey of things, and the overall Expansion and contraction of the apps forces needs to be addressed, Later, We will have a System for it, Already have one for Consistent losing trades to force a timing change up or a freeze.
for (int i = 0; i < 12; i++) {

    Hand* Rh = Righthands[i];
    RMaxWeight += Rh->Weight.Weight;
dots[Rh->IDNUM-1].height = 0;
    if (Rh->Activation) {
 if (RLMODE == 0)
        {
       
            SlidingText(Rh->ID);
        }
        
        //strcat(RactiveIDs, Rh->ID);  // stitch ID to string
        //strcat(RactiveIDs, ". ");
        RScore += Rh->Weight.Weight;
        dots[Rh->IDNUM-1].height = ( .1*Rh->ActivationRate) +5;
        RTotalActive++;
        if (FocusWheelSetting == 1)
        {if (Rh->virt.Human.Active) {
                RScore += 100;}}
        if (FocusWheelSetting == 2)
        {if (Rh->virt.Bomb.Active) {
                RScore += 100;}}
        if (FocusWheelSetting == 3)
        {if (Rh->virt.Babel.Active) {
                RScore += 100;}}
        if (FocusWheelSetting == 4)
        {if (Rh->virt.Industry.Active) {
                RScore += 100;}}
    }
    if (Rh->virt.Human.Active) {
        RMaxWeight += 100;}
    if (Rh->virt.Bomb.Active) {
        RMaxWeight += 100;}
    if (Rh->virt.Babel.Active) {
        RMaxWeight += 100;}
    if (Rh->virt.Industry.Active) {
        RMaxWeight += 100; }// will likely never change this from 100
    
}
BuyPnt = RMaxWeight / 100 * RPercentSelected;
ChangeCheck(RactiveIDs, RpreviousActiveIDs, HandTotal * 4);  // A little broken(activates when it shouldnt, doesnt del old text properly), Check why NOTED 12/17/2025
                                                        // And causes memory leak, will fix later, Noted 1/3/2026
if (RLMODE == 0)//&& (LatestTick - PREVrecvLatestTick) < 10) // I created a Catched Up mechanism But Not I wont be needed, Will be able to backtest lightning fast tho, Or at whatever rate I set.
{ 
if (RScore > BuyPnt)
{Buy = true; //Buy, the bool that determines if the app buys or not
 printf("Buying\r");
}
if (RScore < BuyPnt)
{
    if (Buy == true)
    {
    Buy = false; //Buy, the bool that determines if the app buys or not
    printf("Buy Option Re-evaluated\n");
    }
}
    

}




// * * * SELL SPELL * * * //


LTotalActive = 0;

for (int i = 0; i < 12; i++) {
    Hand* Lh = Lefthands[i];
    LMaxWeight += Lh->Weight.Weight;
   dots[Lh->IDNUM-1].height = 0;
    if (Lh->Activation) {

        if (RLMODE == 1)
        {
            SlidingText(Lh->ID);
        }
        //strcat(LactiveIDs, Lh->ID);  // stitch ID to string
        //strcat(LactiveIDs, ". ");
        LScore += Lh->Weight.Weight;
        dots[Lh->IDNUM-1].height = ( .1*Lh->ActivationRate) +5; // maybe Make this a particaple of Activation rate
        LTotalActive++;

        if (FocusWheelSetting == 1)
        {if (Lh->virt.Human.Active) {
                LScore += 100;}
        }
        if (FocusWheelSetting == 2)
        {if (Lh->virt.Bomb.Active) {
                LScore += 100;}
        }
        if (FocusWheelSetting == 3)
        {if (Lh->virt.Babel.Active) {
                LScore += 100;}
        }
        if (FocusWheelSetting == 4)
        {if (Lh->virt.Industry.Active) {
                LScore += 100;}
        }
    
    }
    if (Lh->virt.Human.Active) {
        LMaxWeight += 100;}
    if (Lh->virt.Bomb.Active) {
        LMaxWeight += 100;}
    if (Lh->virt.Babel.Active) {
        LMaxWeight += 100;}
    if (Lh->virt.Industry.Active) {
        LMaxWeight += 100;} // will likely never change this from 100
}

SellPnt = LMaxWeight / 100 * LPercentSelected;
ChangeCheck(LactiveIDs, LpreviousActiveIDs, HandTotal * 4); // A little broken(activates when it shouldnt, doesnt del old text properly), Check why // CHANGE change check so it just list hands as they activate and DEACTIVATE, IDK  FIX IT
if (RLMODE == 1) //&& (LatestTick - PREVrecvLatestTick) < 10) // I created a Catched Up mechanism But Not I wont be needed, Will be able to backtest lightning fast tho, Or at whatever rate I set. // Oooh catchup mechanism for lag spikes, If errorCode ... no, any catching up will have to be done on the web API. its teh cleanest way
{ 
if (RLMODE == 1)// Well need a stop to make sure its caught up the latest data so we dont insta buy on old data, It should run hella fast when catching up tho so may not matter, //GlobalPTick is exactly as its named, The tick value as given per the API / If its one behind aka on the latest one, Consider buying
{ 
 if (LScore > SellPnt)
{Sell = true; //Buy, the bool that determines if the app buys or not
 printf("Selling\r");
}
if (LScore < SellPnt)
{
    if (Sell == true)
    {
    Sell = false; //Buy, the bool that determines if the app buys or not
    printf("Sell Option Re-evaluated\n");
    }
}
}
}
  //***//***//***//***//***//***//***//***//***//***//***// 
//  // THE // THOUSAND // PALMS // PROCESSING // CODE //  //
  //***//***//***//***//***//***//***//***//***//***//***//


//LatestTick++; //  "LatestTick = GlobalPTick" makes the computational algorithms skip ahead to the latest price tick, LatestTick++; makes it catch up from all prior data, More data good, But Creates desync risk
// DO this main loop, Its a render loop, 
// UPON recieving the new price and volume 
// info, Run the calculation. Then Switch 
// back to the started render loop, Which checks 
// for a new price and renders present shit
//CURRENT goal, make it render animations whilst started,(completed, forgot when, doesnt matter)
      
         // IMPORTANT TO KNOW, the cycle rate of the renderer
}   SDL_Delay(16);
   
SDL_RenderPresent(renderer);  

 // may be the wrong place for this bracket, Fixed a missing bracket error earlier,





 
    SDL_DestroySurface(gui);
    SDL_DestroySurface(StarPloint);
    SDL_DestroySurface(Text);
    SDL_DestroySurface(BlueknobSurface); // Gotta update these
    SDL_DestroySurface(BlueknobSurface);
    SDL_DestroySurface(StartButtonSurface);
    SDL_DestroySurface(StopButtonSurface);
    SDL_DestroySurface(SaveButtonSurface);
    SDL_DestroySurface(SimulButtonSurface);
    SDL_DestroySurface(NullSaveButtonSurface);
    SDL_DestroySurface(FocusWheelZeroSurface);  
    SDL_DestroySurface(FocusWheelOneSurface);  
    SDL_DestroySurface(FocusWheelTwoSurface);   
    SDL_DestroySurface(FocusWheelThreeSurface);      
    SDL_DestroySurface(FocusWheelFourSurface);
    SDL_DestroySurface(BookButtonSurface);  // destroy all the images to fix memeory leak, dont forget that
    SDL_DestroySurface(BookText);  //segfault here
    SDL_DestroySurface(bookSurface);
    SDL_DestroySurface(BookCoverSurface);
    SDL_DestroySurface(BookBasicSurface);
    SDL_DestroySurface(PlointSurface);
    SDL_DestroySurface(HRankS);
    SDL_DestroySurface(HRankA);
    SDL_DestroySurface(HRankB);
    SDL_DestroySurface(HRankC);
    SDL_DestroySurface(HRankD);        
}                                                      
    SDL_DestroyTexture(guitexture);
    SDL_DestroyTexture(StarPlointTexture);
    SDL_DestroyTexture(TextTexture);
    SDL_DestroyTexture(Blueknobtexture);
    SDL_DestroyTexture(RedknobtTexture);
    SDL_DestroyTexture(StartButtonTexture);
    SDL_DestroyTexture(StopButtontexture);
    SDL_DestroyTexture(SaveButtonTexture);
    SDL_DestroyTexture(SimulButtonTexture);
    SDL_DestroyTexture(NullSaveButtonTexture);
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyTexture(FocusZeroTexture);  
    SDL_DestroyTexture(FocusOneTexture);  
    SDL_DestroyTexture(FocusTwoTexture);   
    SDL_DestroyTexture(FocusThreeTexture);      
    SDL_DestroyTexture(FocusFourTexture);
    SDL_DestroyTexture(BookButtonTexture);  
    SDL_DestroyTexture(BookTextTexture);  
    SDL_DestroyTexture(BookCoverTexture); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
//book
    SDL_DestroyTexture(BookTexture);  
    SDL_DestroyRenderer(Bookrenderer); // Destroy all those other textures here too for book
    SDL_DestroyWindow(BOOK);
//book
//closesocket(clientSocket);
   closesocket(serverSocket);
   WSACleanup();

    SDL_Quit();
}
}


    // TO DO LIST
    //FIx rect carry over in Book 
    //Add pace scaler for buttons //Done
    // Handlog, arc Struct getting from text. //Done
    // image related memory leak //DOne
    // Sound, Web api, Exit button not working in book and main
    // Better animation for fogmap //Done
    // Hand Rank Stamp //Done
// Broker Function Complete, 
// Text Read Function/Performence rating function, Get that jump started, Rip from the handlog read function
// THEN SEND TO API FUNCTION
// then cleanup steps, Let the hands activate them selves, we only have Hand 2 active right now

//side note, Couple prices together over a span as a single val, see what kinda changes to the chart that makes

  


   
    
