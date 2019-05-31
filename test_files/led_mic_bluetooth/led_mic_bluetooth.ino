#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 3

//setting up the led strip
CRGB leds[NUM_LEDS];

//setting up the mic
int micPin = 0;
int lower_threshold = 500;
int upper_threshold = 550;

//colour linked list
struct colour_list {
  int data;
  struct colour_list *next;
};
typedef struct colour_list *Node;
Node head = NULL;
int count = 0; //number of items in the linked list
int colour_data[3];

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(60);
}

void loop() {
  /*******************************bluetooth*******************************/
  if (Serial.available() > 0) {
    int incoming = Serial.read();
    Serial.println(incoming);
    add_node(incoming);
  }
  /***********************************************************************/

  /******************************colour data******************************/
  //when 3 colour data have been collected from bluetooth
  if (count == 3) {
    Node temp = head;
    for (int i = 0; temp != NULL; i++) {
      colour_data[i] = temp->data;
      temp = temp->next;
    }

    //for debugging
    for (int i = 0; i < 3; i++) {
      Serial.print(colour_data[i]);
      Serial.print(" ");
    }
    Serial.println("");
    //FastLED.showColor(CRGB(colour_data[0], colour_data[1], colour_data[2])); //display on led
    delete_list(); //reset linked list
  }
  
  /***********************************************************************/

  /**********************************mic**********************************/
  float volume = analogRead(micPin);

  int num_led = map(volume, 0, 1068, 0, NUM_LEDS - 1);
  if (num_led < 0)
    num_led = 0;
  if (num_led > 59)
    num_led = 59;

  Serial.print("volume: ");
  Serial.print(volume);
  Serial.print("  led: ");
  Serial.println(num_led);

  if (volume >= lower_threshold && volume <= upper_threshold) {
    Serial.println("cleared");
    clear_data(); //set to black

  }
  else {
    for (int i = 0; i < num_led; i++) {
      leds[i] = CRGB(colour_data[0], colour_data[1], colour_data[2]);
    }
    FastLED.show();
    Serial.println("show led~~");
  }
  /***********************************************************************/
}

//add node to the colour linked list
void add_node(int incoming) {
  Node newNode, temp;
  newNode = (Node)malloc(sizeof(struct colour_list));
  newNode->data    = incoming;
  newNode->next    = NULL;

  //if the list is empty
  if (head == NULL) {
    head = newNode;
    head->data = incoming;
    head->next = NULL;
    count++;
    Serial.println("count++");
    return;
  }

  //when the list is not empty
  temp = head;

  //traverse till the end of the list and link data
  while (temp->next != NULL)
    temp = temp->next;
  temp->next = newNode;

  //increment number of items in the linked list
  count++;
  Serial.println("count++");
}

//reset colour linked list
void delete_list() {
  Node temp;
  if (head == NULL) {
    printf("Linked list is already empty.\n");
    return;
  }
  while (head != NULL) {
    temp = head->next;
    free(head);
    head = temp;
    count--;
    Serial.println("count--");
  }
}

//clear led strip data (set it to black)
void clear_data() {
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}
