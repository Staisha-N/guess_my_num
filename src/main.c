#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "guess.h"

#define LED_GPIO_PORT GPIOA
#define LED_PIN GPIO_PIN_5

#define BUTTON_GPIO_PORT GPIOC
#define BUTTON_PIN GPIO_PIN_13

const int standard_time = 250;

// const char binary_numbers[10][2][11] = {
//   {"00000000", '0'},
//   {"00000001", '1'},
//   {"00000010", '2'},
//   {"00000011", '3'},
//   {"00000100", '4'},
//   {"00000101", '5'},
//   {"00000110", '6'},
//   {"00000111", '7'},
//   {"00001000", '8'},
//   {"00001001", '9'},
// };

//these functions make the LED blink
void start_game();
void lose();
void win();
void flash_number(int computer_select);

//records the user's guess (their number)
char get_guess();

int count_clicks();

//use to control how long a toggle will last before switching
void delay(int time);

//get the computer's number 
int get_random (int time);

int main();

///////////////////////////
//FUNCTION DECLARATIONS //
/////////////////////////

void start_game () {
    for(int i = 0 ; i < 40; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(50); 
    }
    delay(5000);
}

//flash three times if wrong
void lose() {
    for(int i = 0 ; i < 6; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(120); 
    }
    delay(1000);
}

void next_input_please() {
    for(int i = 0 ; i < 4; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(120); 
    }
    delay(1000);
}

void win(){
    for(int i = 0 ; i < 40; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(50); 
    }
}

void flash_number (int computer_select) {
    for (int i = 0; i < computer_select; i++) {
        //on
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        delay(standard_time);

        //off
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        delay(standard_time);
    }
    delay(5000);
}

char get_guess(){
    //If the user clicks twice, it's 1, if the user clicks once, it's 0
    int guess = 0;
    
    int clicks = count_clicks();

    if (clicks == 2) {
        guess = '1';
    } else if (clicks == 1) {
        guess = '0';
    }

    return guess;
}


//function to count clicks when the button is clicked
int count_clicks(){
    //start the amount of clicks at 0
    int counter = 0;
    //store the current time
    int time_click = HAL_GetTick();

    while (true){
        //while the button is not clicked
        while(HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN)){
            //while true, the button has not been pressed
            //if it's pressed, we exit out of the inside while loop
            //check if the time since the last click is over 1.5s
            if((HAL_GetTick() - time_click) > 1500){
                // HAL_GetTick() is the time since the start
                //time_click is the time since you last clicked the button
                //return the amount of clicks
                return counter;
            }
        }
        //once the button is clicked, increase the amount of clicks
        counter++;
        //set the time that the button is clicked
        time_click = HAL_GetTick();
        //wait until the button is not clicked
        while(!HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN));
        
    }

}


void delay(int time){
    //function to delay (used for on/off for morse code)
    //use instead of normal delay function so it can be broken if the button is clicked
    int counter = 0;
    
    //repeat until the delay is finished (counter = time) or the button is clicked
    while(counter < time && HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN)){
        //increase counter by one and delay 1 ms
        counter+=1;
        HAL_Delay(1);
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
    //every millisecond increases the code by one tick to check the time since the program began
}


int get_random (int time) {
    //the computer picks a random number from 1-10
    srand(time);
    
    int max = 5;

    int random_number = (rand() % max);

    if (random_number == 0) {
        get_random(HAL_GetTick());
    }

    return random_number;
}

int main () {
    HAL_Init();

    SerialSetup(9600);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);

    //try with just one if it bugs
    while(HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN));
    while(HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN));

    start_game();

    int random_number = get_random (HAL_GetTick());
    char buff[100];
    sprintf(buff, "The random number is: %d\r\n", random_number);
    SerialPuts(buff);

    flash_number(random_number);

    next_input_please();
    char num1 = get_guess();
    next_input_please();
    char num2 = get_guess();
    next_input_please();
    char num3 = get_guess();
    next_input_please();
    char num4 = get_guess();
    next_input_please();
    char num5 = get_guess();
    next_input_please();
    char num6 = get_guess();
    next_input_please();
    char num7 = get_guess();
    next_input_please();
    char num8 = get_guess();

    

    // int player_guess_bin [8] = {num1,num2,num3,num4,num5,num6,num7,num8};

    // int guess_zero [8] = {0,0,0,0,0,0,0,0};
    // int guess_one [8] = {0,0,0,0,0,0,0,1};
    // int guess_two [8] = {0,0,0,0,0,0,1,0};
    // int guess_three [8] = {0,0,0,0,0,0,1,1};
    // int guess_four [8]= {0,0,0,0,0,1,0,0};
    // int guess_five [8]= {0,0,0,0,0,1,0,1};
    // char guess_zero [8] = "00000000";
    // char guess_one [8] = "00000001";
    // char guess_two [8] = "00000010";
    // char guess_three [8] = "00000011";
    // char guess_four [8]= "00000100";
    // char guess_five [8]= "00000101";


    

    int player_guess_dec = 0;

    if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '0' && num7 == '0' && num8 == '0') {
        player_guess_dec = 0;
    } else if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '0' && num7 == '0' && num8 == '1') {
        player_guess_dec = 1;
    } else if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '0' && num7 == '1' && num8 == '0') {
        player_guess_dec = 2;
    } else if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '0' && num7 == '1' && num8 == '1') {
        player_guess_dec = 3;
    } else if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '1' && num7 == '0' && num8 == '0') {
        player_guess_dec = 4;
    } else if (num1 == '0' && num2 == '0' && num3 == '0' && num4 == '0' 
        && num5 == '0' && num6 == '1' && num7 == '0' && num8 == '1') {
        player_guess_dec = 5;
    }

    char buff3[100];
    sprintf(buff3, "The user guess is: %d\r\n", player_guess_dec);
    SerialPuts(buff3);

    if (random_number == player_guess_dec) {
        win();
    } else {
        lose();
    }


    // while (true) {
    //     clicks = count_clicks();
    //     if (clicks == random_number) {
    //         win();
    //     } else if (clicks > random_number) {
    //         incorrect_high();
    //     } else if ((clicks < random_number) && (clicks > 0)) {
    //         incorrect_low();
    //     }

    // }

    return 0;
}

/*
int main() {
    HAL_Init();

    SerialSetup(9600);

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN));
    while(HAL_GPIO_ReadPin(GPIOC, BUTTON_PIN));

    //variable to store the number of clicks
    int clicks;
    char string_to_pass;
    int current_difficulty = 1;
    int current_right = 0;
    //seed a random number based on the number of items in the list per difficulty level
    int random = get_random(current_difficulty);


    while(true){

        //use the print_morse function to blink the first value of string_to_pass in morse code
        //chose which list to pull the data from based on difficulty_level

        
        if(current_difficulty == 1)
            print_morse(&shapes_level1[random][0]);
        else if(current_difficulty == 2)
            print_morse(&shapes_level2[random][0]);
        else if(current_difficulty == 3)
            print_morse(&shapes_level3[random][0]);
        // & --> pointer
        //standard_time*7 is the standard delay between words
        delay(standard_time*7);

        //use count_clicks function to set the number of clicks
        clicks = count_clicks();

        //if the number of clicks is equal to the second value of the item in the list

        //random: element in the array, [1]: take the number, [0]: take the index 0 of the number (the whole number is stored in the entry at index zero)
        if((current_difficulty == 1 && clicks == shapes_level1[random][1][0]) || (current_difficulty == 2 && clicks == shapes_level2[random][1][0]) || (current_difficulty == 3 && clicks == shapes_level3[random][1][0])){

            //get a next random numer
            random = get_random(current_difficulty);

            ++current_right;
            //if it's time to move to the next difficulty
            if(current_right ==3 ){
                //reset the current number of correct in a row
                current_right = 0;
                //if all levels are passed
                if(current_difficulty == 3){
                    //display win blinking lights
                    win();
                    //end program
                    break;
                }
                //add one to the current difficulty
                ++current_difficulty;
                random = get_random(current_difficulty);

                //run lights to show moving to the next level
                next_level();
            }
            else{
                correct();
            }
        }
        else if(clicks != 0){
            //run the wrong display
            wrong();
            //get a new random number
            random = get_random(current_difficulty);
            current_right = 0;
        }
        
    }
    return 0;
}


void print_morse(char *string){

  //char string[] = "square"

  //run for each letter in the string
  for(int j = 0; j < strlen(string); j++){
    char character = string[j];

    //run for each letter in the alphabet
    for(int i = 0; i < 26; i++){
      //if the letter in the string equals the letter in the alphabet
      if(morse_alphabet[i][1][0] == character){

        int index = 0;

        while(1){
            //if it's the null character (end of string), break
            if(morse_alphabet[i][0][index] == '\0')
                break;
            else if(morse_alphabet[i][0][index] == '.')
                dot();
            else if(morse_alphabet[i][0][index] == '-')
                dash();

            ++index;
        }
      }
    }

    //use delay function for standard time between character (two)
    delay(standard_time*3);

  }

}

//function to flash a dot (one unit on, one unit off)
void dot(){
    //on
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    delay(standard_time);

    //off
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    delay(standard_time);
}

//function to flash a dash (three units on, one unit off)
void dash(){
    //on
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    delay(standard_time*3);

    //off
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    delay(standard_time);
}


//function to flash a correct (three quick blinks)
void correct(){
    //on
    for(int i = 0 ; i < 6; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(120); 
    }
    delay(1000); 
}

//function to flash next level (four quick blinks)
void next_level(){
    //on
    for(int i = 0 ; i < 8; i++){
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(120); 
    }
    delay(1000); 
}

*/