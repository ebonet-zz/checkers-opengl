struct piece {
        int line,column;
        char pieceType;


        piece(int inputLine, int inputColumn, char inputType){
                this->line = inputLine;
                this->column = inputColumn;
                this->pieceType = inputType;
        }

};

typedef struct piece Piece;
