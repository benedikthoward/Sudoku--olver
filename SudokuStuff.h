//sorts the index into what 3x3 box it is in
int box(int r, int c)
{
    if (r<3)
    {
        if (c<3)
        {
            return 0;
        }else if (c<6)
        {
            return 1;
        }else
        {
            return 2;
        }
    }else if (r<6)
    {
        if (c<3)
        {
            return 3;
        }else if (c<6)
        {
            return 4;
        }else
        {
            return 5;
        }
    }else
    {
        if (c<3)
        {
            return 6;
        }else if (c<6)
        {
            return 7;
        }else
        {
            return 8;
        }
    }
}

int doesNotContain(int a, int yes[80]){
    for(int i=0;i<80;i++){
        if(a==yes[i]){
            return 0;
        }
    }
    return 1;
}
void IndexToVector(int index, int *Xcoordinate, int *Ycoordinate)
{
    int x = index%9;
    int y = (index)/9;
    *Xcoordinate = x;
    *Ycoordinate = y;
}

struct nonRes{
    int numberOfRestrictions;
    int index[80];
};
