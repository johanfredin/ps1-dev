#include "constants.h"

#define JUMP 15
#define MAX_GRAV 10

typedef struct {
	u_short x;
	u_short y;
	u_short width;
	u_short height;
	Image img;
} Sprite;

typedef struct {
	u_short x;
	u_short y;
	u_short width;
	u_short height;
	Box box;
} GameObject;

u_short numBounds = 0;
Sprite raichu; 
Sprite tree;
GameObject objects[2];
GameObject grass;
GameObject bounds;

u_char leftCol, rightCol, topCol, bottomCol;


int main() {
	initialize();

	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 10, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters

	while(1) {
		update();
		draw();
		display();
		clearDisplay();
	}
}

void initialize() {
	initializeScreen();
	initializePad();
	setBackgroundColor(createColor(100, 0, 255));
	
	initializeSprite(&tree, 150, SCREEN_HEIGHT - 74, 64, 64, createImage(img_trees));
	initializeSprite(&raichu, 150, 150, 16, 16, createImage(img_raichu));
	initializeGameObject(&grass, 0, SCREEN_HEIGHT - 10, SCREEN_WIDTH - 1, 10, createColor(0, 255, 0));
	initializeGameObject(&bounds, tree.x, tree.y, tree.width, tree.height, createColor(15, 15, 15));
	objects[0] = grass;
	objects[1] = bounds;
	numBounds = 2;
}

void initializeSprite(Sprite* sprite, int x, int y, int width, int height, Image img) {
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->img = img;
}

void initializeGameObject(GameObject* gobj, int x, int y, int width, int height, Color color) {
	int coordWitdth = x + width;
	int coordHeight = y + height;
	
	gobj->x = x;
	gobj->y = y;
	gobj->width = coordWitdth;
	gobj->height = coordHeight;
	gobj->box = createBox(color, x, y, coordWitdth, coordHeight);
};

short jumpStrength = JUMP;
short acceleration = 1;
short speed = 4;
short gravity = 4;
u_char jumpPressed = 0;

void update() {
	updateGamePadState();
	raichu.y += gravity;
	if(jumpPressed) {
		jump();
	}
	handleCollision();
	raichu.img = moveImage(raichu.img, raichu.x, raichu.y);
	tree.img = moveImage(tree.img, tree.x, tree.y);
	logState();
}

void updateGamePadState() {
	padUpdate();
	if(padCheck(Pad1Down)) {
		raichu.y += speed;
	} 
	if(padCheck(Pad1Up)) {
		raichu.y -= speed;
	}
	if(padCheck(Pad1Left)) {
		raichu.x -= speed;
	}
	if(padCheck(Pad1Right)) {
		raichu.x += speed;
	}
	if(padCheck(Pad1Cross)) {
		jumpPressed = 1;
	}
	if(padCheck(Pad1Start))	{
		raichu.y = 0;
	}
	// drawenv.clip.x = raichu.x;
	// drawenv.ofs[1] = raichu.x + SCREEN_WIDTH;
	// drawenv.ofs[0] = raichu.x;
	dispenv.disp.x = (raichu.x + raichu.width) / 2 - (SCREEN_WIDTH / 2);
	dispenv.screen.x = (raichu.x + raichu.width) / 2 - (SCREEN_WIDTH / 2);
	// dispenv.disp.y = raichu.y
}


void jump() {
	raichu.y -= jumpStrength;
	jumpStrength -= acceleration;
}


void draw() {
	FntFlush(-1);
	drawImage(tree.img, 1);
	drawImage(raichu.img, 0);
	drawBox(grass.box);
	drawBox(bounds.box);
}

int i = 0;
void handleCollision() {
	if((raichu.x + raichu.width) >= SCREEN_WIDTH) {
		raichu.x = (SCREEN_WIDTH - raichu.width);
	} 
	if(raichu.x <= 0) {
		raichu.x = 0;
	}
	if(raichu.y <= 0) {
		raichu.y = 0;
	}

	for(i = 0; i < numBounds; i++) {
		handleBoxCollision(objects[i]);
	}
	i = 0;
}

void handleBoxCollision(GameObject object) {
	if((raichu.x + raichu.width >= object.x) &&
		(raichu.x <= object.x) &&
		(raichu.y + raichu.height > object.y) &&
		(raichu.y < object.height)) { 
		rightCol = 1;
		raichu.x = object.x - raichu.width;
	}
	if((raichu.x <= object.width) &&
	   (raichu.x + raichu.width > object.width) &&
	   (raichu.y + raichu.height > object.y) &&
	   (raichu.y < object.height)) { 
		leftCol = 1;
		raichu.x = object.width;
	}

	topCol = (raichu.y + raichu.height >= object.y) &&
		(raichu.y < object.y) &&
		(raichu.x > object.x) &&
		(raichu.x + raichu.width < object.width);
	if(topCol) { 
		jumpStrength = JUMP;
		jumpPressed = 0;
		raichu.y = object.y - raichu.height;
	}
	if((raichu.y <= object.height) &&
	   (raichu.y + raichu.height > object.height) &&
	   (raichu.x > object.x) &&
	   (raichu.x + raichu.width < object.width)) { 
		bottomCol = 1;
		raichu.y = object.height;
	}

	rightCol=0;
	leftCol=0;
	bottomCol=0;
}

void logState() {
	FntPrint("x=%d, y=%d\nbounds.x=%d, bounds.y=%d\ntopCol:%d\njumping:%d\ngravity:%d\njumpStrength:%d\ndispX:%d, dispY%d, dispw:%d, disph:%d", raichu.x, raichu.y, bounds.x, bounds.y, topCol, jumpPressed, gravity, jumpStrength, dispenv.disp.x, dispenv.disp.y, dispenv.disp.w, dispenv.disp.h);
	
}
