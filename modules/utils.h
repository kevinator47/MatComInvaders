
void GenerateSpaceBlock(char* block, int width) {
    /* Genera un bloque de espacios en blanco de un ancho especifico */
    for (int i = 0; i < width; i++) {
        block[i] = ' ';
    }
    block[width] = '\0';
}
int max(int num1 , int num2)
{
    return (num1 > num2)? num1 : num2;
}

int min(int num1, int num2)
{
    return (num1 < num2)? num1 : num2;
}

int CheckIntersection(int a , int b , int c , int d)
{
    return min(b, d) >= max(a, c);
}