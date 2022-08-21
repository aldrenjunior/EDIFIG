#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

const int MAX=512;

struct Figure
{
    int color;
    char p3[3];
    int nl;
    int nc;
    int fig [MAX][MAX][3];
};

void open_img (char name[], Figure& img)
{
    int i, j, k, nl, nc, color;
    char p3 [3];
    ifstream file;

    file.open(name);
    if (!file.is_open())
    {
        cout << "error opening file!"<< endl;
        exit (0);
    }

    file >> img.p3;
    if(strcmp(img.p3, "P3")!=0)
    {
        cout << "error opening file!"<< endl;
        exit (0);
    }

    file >> img.nc;
    file >> img.nl;
    file >> img.color;

    for (i=0; i<img.nl; i++)
    {
        for(j=0; j<img.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                file >> img.fig[i][j][k];
            }
        }
    }
    file.close();
}

void save_img (char name2[], Figure img)
{
    int i, j, k;

    ofstream new_img;
    new_img.open(name2);

    if (!new_img.is_open())
    {
        cout << "Error: file not saved!\n";
        exit (0);
    }
    else
    {
        cout << "file saved successfully!\n";
    }

    new_img << img.p3 << endl;
    new_img << img.nc << " " << img.nl << endl;
    new_img << img.color << endl;

    for (i=0; i<img.nl; i++)
    {
        for(j=0; j<img.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                new_img << img.fig[i][j][k]<< " ";
            }
        }
        cout << endl;
    }


    new_img.close();
}

void binarizar (Figure img_in, Figure& img_out, int limiar)
{
    int i,j,k;
    float media=0.0;
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                media+=img_in.fig[i][j][k];
            }
            media/=3;
            if(media>=limiar)
            {
                img_out.fig[i][j][0]=255;
                img_out.fig[i][j][1]=255;
                img_out.fig[i][j][2]=255;
            }
            else
            {
                img_out.fig[i][j][0]=0;
                img_out.fig[i][j][1]=0;
                img_out.fig[i][j][2]=0;
            }
            media=0.0;
        }
    }
}

void turn90 (Figure img_in, Figure& img_out)
{
    int i,j,k,aux;
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                img_out.fig[j][img_in.nl - i -1][k]=img_in.fig[i][j][k];
            }
        }
    }
    aux=img_in.nl;
    img_out.nl=img_in.nc;
    img_out.nc=aux;
}

int order_vector (int V[], int n)
{
    int aux;
    int i, j;
    for (i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            if(V[i]>V[j])
            {
                aux=V[i];
                V[i]=V[j];
                V[j]=aux;
            }
        }
    }
}

void Mediana (Figure img_in, Figure& img_out)
{
    int i,j,k;
    int V[MAX];
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {

                if(i==0 && j==0)
                {

                    V[0]=img_in.fig[i][j][k];
                    V[1]=img_in.fig[i][j+1][k];
                    V[2]=img_in.fig[i+1][j][k];
                    V[3]=img_in.fig[i+1][j+1][k];

                    order_vector (V, 4);
                    img_out.fig[i][j][k]=(V[1]+V[2])/2;

                }
                else if (i==0 && j==img_in.nc-1)
                {

                    V[0]=img_in.fig[i][j-1][k];
                    V[1]=img_in.fig[i][j][k];
                    V[2]=img_in.fig[i+1][j-1][k];
                    V[3]=img_in.fig[i+1][j][k];

                    order_vector (V,4);
                    img_out.fig[i][j][k]=(V[1]+V[2])/2;

                }
                else if (i==img_in.nl-1 && j==0)
                {

                    V[0]=img_in.fig[i-1][j][k];
                    V[1]=img_in.fig[i-1][j+i][k];
                    V[2]=img_in.fig[i][j][k];
                    V[3]=img_in.fig[i][j+1][k];

                    order_vector (V,4);

                    img_out.fig[i][j][k]=(V[1]+V[2])/2;

                }
                else if(i==img_in.nl-1 && j==img_in.nc-1)
                {

                    V[0]=img_in.fig[i-1][j-1][k];
                    V[1]=img_in.fig[i-1][j][k];
                    V[2]=img_in.fig[i][j-1][k];
                    V[3]=img_in.fig[i][j][k];

                    order_vector(V,4);

                    img_out.fig[i][j][k]=(V[1]+V[2])/2;

                }
                else if(j==0 && i!=0 && i!=img_in.nl-1)
                {

                    V[0]=img_in.fig[i-1][j][k];
                    V[1]=img_in.fig[i-1][j+i][k];
                    V[3]=img_in.fig[i][j][k];
                    V[4]=img_in.fig[i][j+1][k];
                    V[5]=img_in.fig[i+1][j][k];
                    V[5]=img_in.fig[i+1][j+1][k];

                    order_vector(V,6);

                    img_out.fig[i][j][k]=(V[3]+V[4])/2;

                }
                else if(j==img_in.nc-1 && i!=0 && i!=img_in.nl-1)
                {

                    V[0]=img_in.fig[i-1][j-1][k];
                    V[1]=img_in.fig[i-1][j][k];
                    V[2]=img_in.fig[i][j-1][k];
                    V[3]=img_in.fig[i][j][k];
                    V[4]=img_in.fig[i+1][j-1][k];
                    V[5]=img_in.fig[i+1][j][k];

                    order_vector(V,6);

                    img_out.fig[i][j][k]=(V[3]+V[4])/2;

                }
                else if (i==0 && j!=0 && j!=img_in.nc-1)
                {

                    V[0]=img_in.fig[i][j-1][k];
                    V[1]=img_in.fig[i][j][k];
                    V[2]=img_in.fig[i][j+1][k];
                    V[3]=img_in.fig[i+1][j-1][k];
                    V[4]=img_in.fig[i+1][j][k];
                    V[5]=img_in.fig[i+1][j+1][k];

                    order_vector(V,6);

                    img_out.fig[i][j][k]=(V[3]+V[4])/2;

                }
                else if (i==img_in.nl-1 && j!=0 && j!=img_in.nc-1)
                {

                    V[0]=img_in.fig[i-1][j-1][k];
                    V[1]=img_in.fig[i-1][j][k];
                    V[2]=img_in.fig[i-1][j+i][k];
                    V[3]=img_in.fig[i][j-1][k];
                    V[4]=img_in.fig[i][j][k];
                    V[5]=img_in.fig[i][j+1][k];

                    order_vector(V,6);

                    img_out.fig[i][j][k]=(V[3]+V[4])/2;

                }
                else if (i!=img_in.nl-1 && j!=img_in.nc-1)
                {

                    V[0]=img_in.fig[i-1][j-1][k];
                    V[1]=img_in.fig[i-1][j][k];
                    V[2]=img_in.fig[i-1][j+i][k];
                    V[3]=img_in.fig[i][j-1][k];
                    V[4]=img_in.fig[i][j][k];
                    V[5]=img_in.fig[i][j+1][k];
                    V[6]=img_in.fig[i+1][j-1][k];
                    V[7]=img_in.fig[i+1][j][k];
                    V[8]=img_in.fig[i+1][j+1][k];

                    order_vector(V, 9);

                    img_out.fig[i][j][k]=V[4];
                }
            }
        }
    }
}

void erosion (Figure img_in, Figure& img_out)
{

    int i,j,k,aux;
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                if(img_in.nc==(img_in.nc-1))
                {
                    if(img_in.fig[i][j][k]==255  && img_in.fig[i+1][j][k]==255)
                    {
                        img_out.fig[i][j][k]=255;
                    }
                    else
                    {
                        img_out.fig[i][j][k]=0;
                    }

                }
                else if(img_in.nc!=(img_in.nc-1))
                {
                    if(img_in.fig[i][j][k]==255 && img_in.fig[i][j+1][k]==255 && img_in.fig[i+1][j][k]==255 && img_in.fig[i+1][j+1][k]==255)
                    {
                        img_out.fig[i][j][k]=255;
                    }
                    else
                    {
                        img_out.fig[i][j][k]=0;
                    }
                }
                else if(img_in.nl==(img_in.nl-1))
                {
                    if(img_in.fig[i][j][k]==255 && img_in.fig[i][j+1][k]==255)
                    {
                        img_out.fig[i][j][k]=255;
                    }
                    else
                    {
                        img_out.fig[i][j][k]=0;
                    }


                }
                else if(img_in.nl!=(img_in.nl-1))
                {
                    if(img_in.fig[i][j][k]==255 && img_in.fig[i][j+1][k]==255 && img_in.fig[i+1][j][k]==255 && img_in.fig[i+1][j+1][k]==255)
                    {
                        img_out.fig[i][j][k]=255;
                    }
                    else
                    {
                        img_out.fig[i][j][k]=0;
                    }
                }
            }
        }
    }
}

void noise(Figure img_in, Figure& img_out, int percentual)
{
    srand (time(0)); // inicializa raiz
    int total, quantPixel, i, j, k, Xi,Xj,Xk;
    total = img_in.nl * img_in.nc;
    quantPixel= (percentual/100.0)*total;
    for (i=0; i<quantPixel; i++)
    {
        Xi= (rand() % (img_in.nl)); // (rand() % (q-p+1)) + p;
        Xj= (rand() % (img_in.nc));
        Xk= (rand() % (3) + 1);
        img_out.fig[Xi][Xj][Xk]=255;
    }
    for (j=0; j<quantPixel; j++)
    {
        Xi= (rand() % (img_in.nl)); // (rand() % (q-p+1)) + p;
        Xj= (rand() % (img_in.nc));
        Xk= (rand() % (3) + 1);
        img_out.fig[Xi][Xj][Xk]=0;
    }
}

void binarize_with_mask(Figure img_in, Figure& img_out, int limiar)
{
    int i,j,k,aux;
    float media;
    Figure mask;
    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                if(j<=img_in.nc/2)
                {
                    mask.fig[i][j][k]=0;
                }
                else
                {
                    mask.fig[i][j][k]=255;
                }
            }
        }
    }

    for (i=0; i<img_in.nl; i++)
    {
        for(j=0; j<img_in.nc; j++)
        {
            for (k=0; k<3; k++)
            {
                if(mask.fig[i][j][k]==255)
                {
                    media+=img_in.fig[i][j][k];
                    if(k==2)
                    {
                        media/=3;
                        if(media>=limiar)
                        {
                            img_out.fig[i][j][0]=255;
                            img_out.fig[i][j][1]=255;
                            img_out.fig[i][j][2]=255;
                        }
                        else
                        {
                            img_out.fig[i][j][0]=0;
                            img_out.fig[i][j][1]=0;
                            img_out.fig[i][j][2]=0;
                        }
                        media=0.0;
                    }

                }
            }
        }
    }
}


int main()
{
    int resp, num, num2, num3;
    cout <<"    WELCOME TO EDIFIG " << endl;
    cout << "Do you want to use our program? " << endl;
    cout << "(1) Y " << endl;
    cout << "(2) N " << endl;
    cout << " - ";
    cin >> resp;

    while (resp != 1 && resp != 2)
    {
        cout << "Invalid number, please re-enter: " << endl;
        cout << " - ";
        cin >> resp;
    }

    while(resp == 1)
    {

        cout << endl;
        cout <<"To perform an operation in EDIGIF, enter its respective number. To exit the program press (0) " << endl;
        cout << endl;
        cout << "(1) - Binarization " << endl;
        cout << "(2) - Horizontal Rotation " << endl;
        cout << "(3) - Erosion " << endl;
        cout << "(4) - Median filtering " << endl;
        cout << endl;
        cout << " - ";
        cin >> num;
        cin.ignore();

        while(num != 0 && num != 1 && num != 2 && num != 3 && num !=4 && num !=5)
        {
            cout << "Please enter a valid number: " << endl;
            cin >> num;
            cin.ignore();
        }

        Figure img;
        int limiar, percentual;
        char name[MAX];
        char name2 [MAX];

        switch (num)    //MAIN
        {

        case 1: //MAIN
            cout << endl;
            cout << "Do you want to perform normal or mask binarization?" << endl;
            cout << "(1) Normal binarization " << endl;
            cout << "(2) Mask binarization " << endl;
            cin >> num2;
            cin.ignore();

            while(num2 != 1 && num2 != 2)
            {
                cout << "Please enter a valid number: " << endl;
                cin >> num2;
                cin.ignore();
            }

            switch (num2) //SECUNDARIO
            {
                case 1:
                cout << endl;
                cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
                cin.getline(name, MAX);
                open_img (name, img);
                cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension: " << endl;
                cin.getline(name2, MAX);
                cout << "- To perform the binarization, enter a value for the threshold: " << endl;
                cin >> limiar;
                binarizar(img, img, limiar);
                save_img (name2, img);
                break;

                case 2:
                cout << endl;
                cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
                cin.getline(name, MAX);
                open_img (name, img);
                cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
                cin.getline(name2, MAX);
                cout << "- To perform the binarization with the mask, enter a value for the threshold: " << endl;
                cin >> limiar;
                Binarize_with_mask(img, img, limiar);
                save_img (name2, img);
                break;
            }

        break;

        case 2://MAIN
            cout << endl;
            cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
            cin.getline(name, MAX);
            open_img (name, img);
            cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
            cin.getline(name2, MAX);
            turn90(img, img);
            save_img (name2, img);
            break;

        case 3://MAIN
            cout << endl;
            cout << "Is the file already binarized? " << endl;
            cout << "(1) Y " << endl;
            cout << "(2) N " << endl;
            cout << " - ";
            cin >> num2;
            cin.ignore();

            while(num2 != 1 && num2 != 2)
            {
                cout << "Please enter a valid number: " << endl;
                cout << " - ";
                cin >> num2;
                cin.ignore();
            }

            switch(num2) //SECUNDARIO
            {

            case 1:
                cout << endl;
                cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
                cin.getline(name, MAX);
                open_img (name, img);
                cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
                cin.getline(name2, MAX);
                erosion(img, img);
                save_img (name2, img);
                break;

            case 2:
                    cout << endl;
                    cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
                    cin.getline(name, MAX);
                    open_img (name, img);
                    cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
                    cin.getline(name2, MAX);
                    cout << "- To perform the binarization, enter a value for the threshold: " << endl;
                    cin >> limiar;
                    binarizar(img, img, limiar);
                    erosion(img, img);
                    save_img (name2, img);
                    break;
                }
        break;

        case 4://MAIN
            cout << endl;
            cout << "Is your image already noisy?" << endl;
            cout << "(1) Y " << endl;
            cout << "(2) N " << endl;
            cout << " - ";
            cin >> num2;
            cin.ignore();

            while(num2 != 1 && num2 != 2)
            {
                cout << "Please enter a valid number: " << endl;
                cout << " - ";
                cin >> num2;
                cin.ignore();
            }

            switch(num2) //SECUNDARIO
            {
            case 1:
            cout << endl;
            cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
            cin.getline(name, MAX);
            open_img (name, img);
            cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
            cin.getline(name2, MAX);
            Mediana(img,img);
            save_img (name2, img);
            break;

            case 2:
            cout << endl;
            cout << "- Please enter the name of the file to be edited with the extension '.ppm': " << endl;
            cin.getline(name, MAX);
            open_img (name, img);
            cout << "- Please enter the name of the file to be saved followed by the '.ppm' extension:: " << endl;
            cin.getline(name2, MAX);
            cout << "Please enter the percentage (%) to be used in noise projection: " << endl;
            cin >> percentual;
            noise(img,img, percentual);
            Mediana(img,img);
            save_img (name2, img);
            break;

            }

            break;

        case 0:
            resp = 0;
            break;

        }

    }

    cout << "\nThanks for using! " << endl;
    cout << "End of program!";

    return 0;
}
