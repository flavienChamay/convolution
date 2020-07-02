#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "limace.h"
#include "tai.h"
#include <stdbool.h>

#define DEBOGAGE

#ifdef DEBOGAGE
#define DEBUG fprintf(stderr,"Fichier %s, ligne %d\n",__FILE__,__LINE__);
#else
#define DEBUG
#endif

#define AFAIRE(fonction) fprintf(stderr,"Corps de la fonction " #fonction \
                                        " à écrire.\n" ); \
												 return NULL;




/*
 * Inversion d'une image
 * Entrée : image initiale (binaire, niveaux de gris ou couleur)
 * Sortie : image résultat
 */
Image Inversion(Image Im)
{
	//If the color type is BitMap then 
		//for each pixel, if the image in input is 0 then 1 else 0
	if(ImType(Im) == BitMap){
		unsigned char **getBitMap = ImGetI(Im);
		for(int i = 0; i < ImNbRow(Im); i++){
			for(int j = 0; j < ImNbCol(Im); j++){
				getBitMap[i][j] = (getBitMap[i][j] == 1) ? 0 : 1;
			}
		}
	}
	//If the color type is GreyLevel then
		//for each pixel i, f(i) = 255 - valueOf(i)
	if(ImType(Im) == GrayLevel){
		unsigned char **getGreyLevel = ImGetI(Im);
		for(int i = 0; i < ImNbRow(Im); i++){
			for(int j = 0; j < ImNbCol(Im); j++){
				getGreyLevel[i][j] = 255 - getGreyLevel[i][j];
			}
		}
	}
	//If the color type is Color then
		//for each pixel i, and each color RGB of the pixel, f(i) = 255 - valueOf(i)
	if(ImType(Im) == Color){
		unsigned char **getColorB = ImGetB(Im);
		unsigned char **getColorG = ImGetG(Im);
		unsigned char **getColorR = ImGetR(Im);
		for(int i = 0; i < ImNbRow(Im); i++){
			for(int j = 0; j < ImNbCol(Im); j++){
				getColorB[i][j] = 255 - getColorB[i][j];
				getColorG[i][j] = 255 - getColorG[i][j];
				getColorR[i][j] = 255 - getColorR[i][j];
			}
		}
	}
	return Im;
}

/**
 * Find the minimum in an array of unsigned char
 * INPUT: UNISGNED CHAR ** X INT X INT
 * OUTPUT: UNSIGNED CHAR
 * */
unsigned char min(unsigned char ** tab, int row, int column){
	unsigned char minimum = tab[0][0];
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			if(tab[i][j] < minimum){
				minimum = tab[i][j];
			}
		}
	}
	return minimum;
}

/**
 * Find the maximum in an array of unsigned char
 * INPUT: UNISGNED CHAR ** X INT X INT
 * OUTPUT: UNSIGNED CHAR
 * */
unsigned char max(unsigned char ** tab, int row, int column){
	unsigned char maximum = tab[0][0];
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			if(tab[i][j] > maximum){
				maximum = tab[i][j];
			}
		}
	}
	return maximum;
}

 /*
  * Étalement de la dynamique d'une image de niveaux de gris
  * Entrée : image initiale en niveaux de gris
  * Sortie : image résultat
  */
Image Etalement(Image Im)
{
	unsigned char **getGreyLevel = ImGetI(Im);
	unsigned char m = min(getGreyLevel, ImNbRow(Im), ImNbCol(Im));
	unsigned char M = max(getGreyLevel, ImNbRow(Im), ImNbCol(Im));
	
	//For each pixel i in Im, apply f(i) = [255 * (valueOf(i) - m)/(M - m) ]
	for(int i = 0; i < ImNbRow(Im); i++){
		for(int j = 0; j < ImNbCol(Im); j++){
			getGreyLevel[i][j] = round(255 * ((float)getGreyLevel[i][j] - (float)m)/((float)M - (float)m));
		}
	}
	return Im;
}

/*
 * Binarisation d'une image de niveaux de gris par seuillage global
 * Entrées : image de niveaux de gris initiale et seuil
 * Sortie : image binaire
 */
Image Binarisation(Image Im, unsigned char Seuil)
{
	unsigned char **getGreyLevel = ImGetI(Im);
	for(int i = 0; i < ImNbRow(Im); i++){
		for(int j = 0; j < ImNbCol(Im); j++){
			getGreyLevel[i][j] = (getGreyLevel[i][j] < Seuil) ? 0 : 255;
		}
	}
	return Im;
}

/**
 * Computes the average of all values in the radius of the mask
 * INPUT: UNSIGNED CHAR ** x INT x INT x INT
 * OUTPUT: UNSIGNED CHAR
 * */
unsigned char calculMoyenne(unsigned char **tab, int sizeMask, int row, int col){
	float result = 0;
	//for each pixel in the range of the mask we sum all of those pixels in tab
	for(int i = 0; i < sizeMask; i++){
		for(int j = 0; j < sizeMask; j++){
			result = result + (float)tab[row - sizeMask /2 + i][col - sizeMask / 2 + j];
		}
	}
	//we divide by the number of pixels and we obtain the average
	return (unsigned char)round(result / (sizeMask * sizeMask));
}

/*
 * Filtrage moyen d'une image de niveaux de gris
 * Entrées : image initiale en niveaux de gris
 *           taille du voisinage (Taille x Taille)
 * Sortie : image résultat
 */
Image Moyen(Image Im, int Taille)
{
	int radiusMask = Taille / 2; 
	int endRow = ImNbRow(Im) - radiusMask;
	int endCol = ImNbCol(Im) - radiusMask;
	//If the mask exceed the image then we return the image without modification
	if (endRow <= radiusMask || endCol <= radiusMask || endRow <= 0 || endCol <= 0){
		return Im;
	}
	
	//If not we modify the image
	unsigned char **getGreyLevelIm = ImGetI(Im);
	Image imgResMoyen = ImCopy(Im);
	unsigned char **getGreyLevelResMoy = ImGetI(imgResMoyen);
	//For each pixel that the mask doesn't exceed the image then we compute the average 
	for(int i = radiusMask; i < endRow ; i++){
		for(int j = radiusMask; j < endCol; j++){
			getGreyLevelResMoy[i][j] = calculMoyenne(getGreyLevelIm, Taille, i, j);
		}
	}
	return imgResMoyen;
}


/**
 * Compare two unsigned char and returns the smaller of the two
 * INPUT: CONST VOID* x CONST VOID *
 * OUTPUT: INT
 * */
int comparaisonMedian(const void * a, const void * b) {
	return (int)(*(unsigned char*)a - *(unsigned char*)b);
}

/**
 * Compute the median of a set of pixels as a function of the mask
 * INPUT: UNISGNED CHAR x INT x INT x INT
 * OUTPUT: UNSIGNED CHAR
 * */
unsigned char calculMedian(unsigned char **tab, int sizeMask, int row, int col){
	unsigned char * median = (unsigned char *)malloc(sizeof(unsigned char) * sizeMask * sizeMask);
	for(int i = 0; i < sizeMask; i++){
		for(int j = 0; j < sizeMask; j++){
			median[i * sizeMask + j] = tab[row - sizeMask /2 + i][col - sizeMask / 2 + j];
		}
	}
	qsort(median,(sizeMask * sizeMask), sizeof(unsigned char), comparaisonMedian);
	return median[(sizeMask*sizeMask) / 2];
}

unsigned char calculHisto(unsigned char **tab, int sizeMask, int row, int col){
	unsigned char * median = (unsigned char *)malloc(sizeof(unsigned char) * sizeMask * sizeMask);
	int H[256]; //represents the histogram of the image 
	int C[256]; //represents the cumulative histogram of the image
	for(int i = 0; i < sizeMask; i++){
		for(int j = 0; j < sizeMask; j++){
			median[i * sizeMask + j] = tab[row - sizeMask /2 + i][col - sizeMask / 2 + j];
		}
	}
	//filling H and C with zeros
	for(int i = 0; i < 256; i++){
		H[i] = 0;
		C[i] = 0;
	}
	//compute H
	for(int i = 0; i < sizeMask * sizeMask; i++){
		H[(int)median[i]]++;
	}
	//compute C and stop when we are on the median
	C[0] = H[0];
	int temp = 0;
	int i = 1;
	while(temp == 0){
		C[i] = C[i-1] + H[i];
		if(C[i] >= ((sizeMask * sizeMask) / 2) + 1){
			break;
		}
		i++;
	}
	
	return (unsigned char)i;
}

/*
 * Filtrage médian d'une image de niveaux de gris
 * Entrées : image initiale en niveaux de gris
 *           taille du voisinage (Taille x Taille)
 * Sortie : image résultat
 */
Image Median(Image Im, int Taille)
{
	int radiusMask = Taille / 2; 
	int endRow = ImNbRow(Im) - radiusMask;
	int endCol = ImNbCol(Im) - radiusMask;
	int choice; //filter the decision to use sort method or histogram method
	//If the mask exceed the image then we return the image without modification
	if (endRow <= radiusMask || endCol <= radiusMask || endRow <= 0 || endCol <= 0){
		return Im;
	}
	unsigned char **getGreyLevelIm = ImGetI(Im);
	Image imgResMedian = ImCopy(Im);
	unsigned char **getGreyLevelResMed = ImGetI(imgResMedian);
	
	printf("Type 0 to use sort method or whatever you want to use the histogram method");
	scanf("%d",&choice);
	//For each pixel that the mask doesn't exceed the image then we compute the median
	for(int i = radiusMask; i < endRow; i++){
		for(int j = radiusMask; j < endCol; j++){
			getGreyLevelResMed[i][j] = 
				(choice == 0) ? 
				calculMedian(getGreyLevelIm, Taille, i, j)
				:
				calculHisto(getGreyLevelIm, Taille, i, j);
		}
	}
	return imgResMedian;
}

/*
 * Vérification de la validité d'une matrice représentant un élément structurant
 * Entrée : matrice représentant un élément structurant
 * Sortie : 0 si la matrice est valide,
            ES_NOT_ODD si son nombre de lignes ou de colonnes n'est pas impair
            ES_NOT_INT si elle ne contient pas que des entiers
            ES_NOT_BIN si elle ne contient pas que des 0 et des 1
*/
int InvalideES(Matrix ElementStructurant)
{
  int **ES,NbLig,NbCol,i,j;

  if (MatType(ElementStructurant)!=Int)
    return ES_NOT_INT;
  NbLig=MatNbRow(ElementStructurant);
	if ((NbLig%2)!=1)
	  return ES_NOT_ODD;
  NbCol=MatNbCol(ElementStructurant);
	if ((NbCol%2)!=1)
	  return ES_NOT_ODD;
  ES=MatGetInt(ElementStructurant);
  for (i=0;i<NbLig;i++)
    for (j=0;j<NbCol;j++)
      if (ES[i][j]!=0 && ES[i][j]!=1)
        return ES_NOT_BIN;
  return 0;
}

/*
 * Érosion d'une image binaire
 * Entrées : image binaire initiale et élément structurant (matrice de int
 * contenant uniquement des 0 et des 1)
 * Sortie : image binaire erodée
 */
Image Erosion(Image Im, Matrix ElementStructurant)
{
	//Copy of the input image, serving as a reference
	Image imgRes = ImCopy(Im);
	unsigned char **getGreyLvlImRef = ImGetI(Im);
	//Input result image 
	unsigned char **getGreyLvlImRes = ImGetI(imgRes);
	
	int **getMatElemStruct = MatGetInt(ElementStructurant);
	
	//compute center of the matrix
	int centerRowMask = MatNbRow(ElementStructurant) / 2;
	int centerColMask = MatNbCol(ElementStructurant) / 2;
	
	//For each pixel in the image
	for(int i = 0; i < ImNbRow(Im); i++){
		for(int j = 0; j < ImNbCol(Im); j++){
			//If that pixel is a 1 then we trigger the erosion
			if(getGreyLvlImRef[i][j] == 1){
				//Variable who tells if we transform the image or not
				bool temp = true;
				//Variable that breaks for loops for saving time
				bool breakFor = false;
				//For each pixel in the mask
				for(int k = 0; k < MatNbRow(ElementStructurant); k++){
					if(breakFor){break;}
					for(int l = 0; l < MatNbCol(ElementStructurant); l++){
						if(breakFor){break;}
						//If the pixel at 1 in the mask corresponds to a 0 in the image then we return false immediatly
						//Compute distance from center of mask to current pixel of mask
						int distRowCenterPixMask = k - centerRowMask;
						int distColCenterPixMask = l - centerColMask;
						//compute coordoninates of pixel of the image corresponding to the mask
						int coordImgRow = i + distRowCenterPixMask;
						int coordImgCol = j + distColCenterPixMask;
						//If the coordinates are positive and lesser than the number of lines and columns
						if(coordImgCol >= 0 && coordImgRow >= 0 && coordImgRow < ImNbRow(Im) && coordImgCol < ImNbCol(Im)){
							//If there is a pixel at 1 in the mask that corresponds to a pixel at 0 in the image then false else true
							if(getMatElemStruct[k][l] == 1 && getGreyLvlImRef[coordImgRow][coordImgCol] == 0){
								temp = false;
								breakFor = true;
							}
							else{
								temp = temp && true;
							}
						}
					}
				}
				//If the condition is not met then the pixel of the image is set to 0
				if(! temp){
					getGreyLvlImRes[i][j] = 0;
				}
			}
		}
	}
	return imgRes;
}

/*
 * Dilatation d'une image binaire
 * Entreés : image binaire initiale et élément structurant (matrice de int
 * contenant uniquement des 0 et des 1)
 * Sortie : image binaire dilatée
 */
Image Dilatation(Image Im, Matrix ElementStructurant)
{
	//Copy of the input image, serving as a reference
	Image imgRes = ImCopy(Im);
	unsigned char **getGreyLvlImRef = ImGetI(Im);
	//Input result image 
	unsigned char **getGreyLvlImRes = ImGetI(imgRes);
	
	int **getMatElemStruct = MatGetInt(ElementStructurant);
	
	//compute center of the matrix
	int centerRowMask = MatNbRow(ElementStructurant) / 2;
	int centerColMask = MatNbCol(ElementStructurant) / 2;
	
	//For each pixel in the image
	for(int i = 0; i < ImNbRow(Im); i++){
		for(int j = 0; j < ImNbCol(Im); j++){
			//If the pixel is equal to 0 then we trigger the dilatation
			if(getGreyLvlImRef[i][j] == 0){
				//Variable who tells if we transform the image or not
				bool temp = false;
				//For each pixel in the mask
				for(int k = 0; k < MatNbRow(ElementStructurant); k++){
					for(int l = 0; l < MatNbCol(ElementStructurant); l++){
						//Compute distance from center of mask to current pixel of mask
						int distRowCenterPixMask = k - centerRowMask;
						int distColCenterPixMask = l - centerColMask;
						//compute coordoninates of pixel of the image corresponding to the mask
						int coordImgRow = i + distRowCenterPixMask;
						int coordImgCol = j + distColCenterPixMask;
						//If the coordinates are positive and lesser than the number of lines and columns
						if(coordImgCol >= 0 && coordImgRow >= 0 && coordImgRow < ImNbRow(Im) && coordImgCol < ImNbCol(Im)){
							//If there is a pixel at 1 in the mask that corresponds to a pixel at 0 in the image then false else true
							if(getGreyLvlImRef[coordImgRow][coordImgCol] == 1 && getMatElemStruct[k][l] == 1){
								temp = true;
							}
							else{
								temp = temp || false;
							}
						}
					}
				}
				getGreyLvlImRes[i][j] = (temp) ? 1 : 0;
			}
		}
	}
	return imgRes;
}
