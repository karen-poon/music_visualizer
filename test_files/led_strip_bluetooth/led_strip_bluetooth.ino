#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 3

//setting up the led strip
CRGB leds[NUM_LEDS];

//colour linked list
struct colour_list {
  int data;
  struct colour_list *next;
};
typedef struct colour_list *Node;
Node head = NULL;
int count = 0; //number of items in the linked list

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(60);
}

void loop() {
  if (Serial.available() > 0) {
    int incoming = Serial.read();
    Serial.println(incoming);
    add_node(incoming);
  }
  //when 3 colour data have been collected from bluetooth
  if (count == 3) {
    Node temp = head;
    int colour_data[3];
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
    
    FastLED.showColor(CRGB(colour_data[0], colour_data[1], colour_data[2])); //display on led
    delete_list(); //reset linked list
  }
}

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
