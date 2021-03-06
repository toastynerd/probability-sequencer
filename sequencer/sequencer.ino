/*  probability sequencer
    eurorack arduino project
    toastynerd<Tyler Morgan> 2021
    intended to use with arduino nano but would work
    with other compatible boards as long as the starting
    pins are adjusted. 
    main.ino
*/

#define ENTROPY A0
#define CLOCK 12
#define ANALOG_PINS 7
#define NUMBER_OF_NOTES 4
#define NOTE_START_PIN 2

static const uint8_t analog_pins[] = {A1, A2, A3, A4, A5, A6, A7};

typedef struct NoteProb {
	int start_val;
	int end_val;
};

int get_choice(int max_val) {
	//writing this method as future proofing a control voltage based
	//choice selection
	return random(0, max_val);
};

int get_new_note() {
	NoteProb probabilities[NUMBER_OF_NOTES];
	int x;
	int choice;
	int current_total = 0;
	int read_val;

	for(x = 0; x < NUMBER_OF_NOTES; x++) {
		probabilities[x].start_val = current_total;
		read_val = analogRead(analog_pins[x]);
		if (read_val < 5) {
			read_val = 0; //discarding super small values because the real world is weird
		}
		current_total += read_val;
		probabilities[x].end_val = current_total;
	}

	choice = get_choice(current_total);

	for(x = 0; x < NUMBER_OF_NOTES; x++) {
		if (choice > probabilities[x].start_val && choice < probabilities[x].end_val) {
			Serial.println("start_val: ");
			Serial.println(probabilities[x].start_val); 
			Serial.println("end_val: ");
			Serial.println(probabilities[x].end_val);
			Serial.println("choice: ");
			Serial.println(choice);
			return x;
		}
	}
	return -1;
}

void clear_notes() {
	for(int x = 0; x < NUMBER_OF_NOTES; x ++) {
		digitalWrite(NOTE_START_PIN + x, LOW);
	}

}

void set_note(int note) {
	digitalWrite(NOTE_START_PIN + note, HIGH);
}

void setup() {
	Serial.begin(9600);
	pinMode(ENTROPY, INPUT);
	pinMode(CLOCK, INPUT);
	int x;
	for(x = 0; x < ANALOG_PINS; x++) {
		pinMode(analog_pins[x], INPUT);
	}

	for(x = 0; x < NUMBER_OF_NOTES; x++) {
		pinMode(NOTE_START_PIN + x, OUTPUT);
	}
	randomSeed(analogRead(ENTROPY));
}

void loop() {
	int newNote = get_new_note();
	Serial.println(newNote);
	clear_notes();
	if (newNote != -1) {
		set_note(newNote);
	}
	delay(1000);
}
