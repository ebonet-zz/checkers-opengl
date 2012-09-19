struct piece {
	int x,y;
	char pieceType;
	GLfloat color[4];


	piece(int inputX, int inputY, char inputType){
		this->x = inputX;
		this->y = inputY;
		setColor(inputType);
	}

	void setColor(char pieceType){
		if(pieceType == 'b' || pieceType == 'B')
			color = {0.0,0.0,0.0,1.0};
		else
			color = {0.9,0.2,0.2,1.0};
	}
};
