int max(int num1 , int num2)
{
    return (num1 > num2)? num1 : num2;
}

int min(int num1, int num2)
{
    return (num1 < num2)? num1 : num2;
}
void GetMovement(char c, int* mov_x, int* mov_y) {
    *mov_x = *mov_y = 0;
    switch (c)
    {
    case 'w':
        *mov_y = -1;
        break;
    case 's':
        *mov_y = 1;
        break;
    case 'a':
        *mov_x = -1;
        break;
    case 'd':
        *mov_x = 1;
        break;
    default:
        break;
    }
}