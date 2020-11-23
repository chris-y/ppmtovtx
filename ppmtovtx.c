/************************************************
 * ppmtovtx v1.3                                *
 * (c) 2002 Unsatisfactory Software/Chris Young *
 * http://www.unsatisfactory.freeserve.co.uk    *
 *                                              *
 * All use of this program is at your own risk. *
 * No modified binaries of this code may be     *
 * distributed without the modifed source code  *
 * without the express permission of the author *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    unsigned char esc=27,data[17281],screen[17281],colour[9],base=80,bkg=0,sepc=90,holdc=94,bkgc=93,bkgr=92;
    int threshold,count,count2,ptrd,ptrs,mosaic,result,textdata,chars,best,a,pbest=0,height,datlen;
    char *hold=0,*bkga=0,*sep=0,*tmp,*format=0,*nbf=0;
    char *ver = "ppmtovtx 1.3 by Chris Young <chris@unsatisfactorysoftware.co.uk>\0$VER: ppmtovtx 1.3 (01.08.2002)\0$STACK:50000\0";

	threshold=170; /*50;*/ /* 180; */

    printf("%s\n\n",ver);

    if(argc<3)
    {
        printf("ppmtovtx <input file> <output file> [options [threshold]]\n\n");
        printf("Where:   input file  80 pixel wide binary PPM (P6), 69 pixel height for 23 rows\n");
        printf("         output file Filename to save output as, use extension .bin or .vtx\n");
        printf("         options     One or more of {SHADKRGYBMCW} (see below)\n");
        printf("         threshold   Colour detection level 0-255\n\n");
		  printf("Options: S Separate                        K Black background (default)\n");
		  printf("         H Held graphics                   R Red background\n");
		  printf("         A Automatic background colour     G Green background\n");
		  printf("         D Disallow black foreground       Y Yellow background\n");
		  printf("                                           B Blue background\n");
		  printf("                                           M Magenta background\n");
		  printf("                                           C Cyan background\n");
		  printf("                                           W White background\n\n");
        exit(0);
    }
    else
    {
    	if(argc>=4)
    	{
    		nbf=strchr(argv[3],'D');
    		sep=strchr(argv[3],'S');
    		hold=strchr(argv[3],'H');
			tmp=strchr(argv[3],'K');
			if(tmp!=0) bkg=0;
			tmp=strchr(argv[3],'R');
			if(tmp!=0) bkg=1;
			tmp=strchr(argv[3],'G');
			if(tmp!=0) bkg=2;
			tmp=strchr(argv[3],'Y');
			if(tmp!=0) bkg=3;
			tmp=strchr(argv[3],'B');
			if(tmp!=0) bkg=4;
			tmp=strchr(argv[3],'M');
			if(tmp!=0) bkg=5;
			tmp=strchr(argv[3],'C');
			if(tmp!=0) bkg=6;
			tmp=strchr(argv[3],'W');
			if(tmp!=0) bkg=7;
			bkga=strchr(argv[3],'A');
    	}

    	if(argc==5)
    	{
    		threshold=atoi(argv[4]);
    	}
        printf("Converting %s to %s...\n",argv[1],argv[2]);
        fp1=fopen(argv[1],"rb");
        if(fp1==NULL)
        {
            printf("Cannot open %s\n",argv[1]);
            exit(1);
        };
        fp2=fopen(argv[2],"wb");
        if(fp2==NULL)
        {
            printf("Cannot open %s\n",argv[2]);
				fclose(fp1);
            exit(1);
        };


		  format = strstr(argv[2],".vtx");

		  if(format==NULL)
		  {
		  	/* user wants to convert to non-ESC code (.bin/.vid) format */
            base=16;
            sepc=26;
            holdc=30;
            bkgc=29;
            bkgr=28;
		  }

			/* read header */
        result=fread(data,13,1,fp1);
        if(result!=1)
        {
            printf("Error reading %s\n",argv[1]);
				fclose(fp1);
				fclose(fp2);
            exit(1);
        };

			height=atoi(&data[6]);
         datlen=240*height;  /*17280 for 72pix height*/
			height=height/3;
        printf("Reading PPM file... [40x%d]\n",height);
        result=fread(screen,datlen,1,fp1);
        if(result!=1)
        {
            printf("Error reading %s\n",argv[1]);
				fclose(fp1);
				fclose(fp2);
            exit(1);
        };

        printf("Sorting data...\n");
        ptrd=0;
        ptrs=0;
        for(count=0;count<height;count++)
        {
            for(count2=0;count2<40;count2++)
            {
                data[ptrd]=screen[ptrs];
                data[ptrd+1]=screen[ptrs+1];
                data[ptrd+2]=screen[ptrs+2];
                data[ptrd+3]=screen[ptrs+3];
                data[ptrd+4]=screen[ptrs+4];
                data[ptrd+5]=screen[ptrs+5];
                data[ptrd+6]=screen[ptrs+240];
                data[ptrd+7]=screen[ptrs+241];
                data[ptrd+8]=screen[ptrs+242];
                data[ptrd+9]=screen[ptrs+243];
                data[ptrd+10]=screen[ptrs+244];
                data[ptrd+11]=screen[ptrs+245];
                data[ptrd+12]=screen[ptrs+480];
                data[ptrd+13]=screen[ptrs+481];
                data[ptrd+14]=screen[ptrs+482];
                data[ptrd+15]=screen[ptrs+483];
                data[ptrd+16]=screen[ptrs+484];
                data[ptrd+17]=screen[ptrs+485];

                ptrd=ptrd+18;
					ptrs=ptrs+6;
            };
            ptrs=ptrs+480;
			};

        printf("Mapping colours...\n");

        for(count=0;count<datlen;count=count+3)
        {

/* colours:
   0=black
   1=red
   2=green
   3=yellow
   4=blue
   5=magenta
   6=cyan
   7=white
*/

textdata=0;

            if(data[count]>threshold) /* red component */
            {
	            textdata=1;
	         }
            if(data[count+1]>threshold) /* green */
            {
	            textdata=textdata+2;
	         }
            if(data[count+2]>threshold) /* blue */
            {
	            textdata=textdata+4;
	         }

	        	screen[count/3]=textdata;

			};

			count2=0;
			chars=0;

        for(count=0;count<(240*height);count=count+6) /* 5760 */
        {

if(chars==0)
{
	if(sep!=NULL)
	{
			if(format!=NULL)
			{
            data[count2]=esc;
            count2++;
			}
            data[count2]=sepc;
            count2++;
            chars++;
            count=count+6;
	}
	if(hold!=NULL)
	{
			if(format!=NULL)
			{
            data[count2]=esc;
            count2++;
			}
            data[count2]=holdc;
            count2++;
            chars++;
            count=count+6;
	}

	if(bkg!=0)
	{
			if(format!=NULL)
			{
				data[count2]=esc;
				count2++;
			}
			data[count2]=(base+bkg);
			count2++;
			chars++;
			count=count+6;
			pbest=best;

			if(format!=NULL)
			{
            data[count2]=esc;
            count2++;
			}
            data[count2]=bkgc;
            count2++;
            chars++;
            count=count+6;
	}
pbest=0;
}

/* find colour */

/* best=bkg+1; */

if(best>7) best=0;

for(a=0;a<9;a++)
{
	colour[a]=0;
}

for(a=0;a<6;a++)
{
	colour[screen[count+6+a]]++; /* we need to check the next block along */
}

for(a=0;a<8;a++)
{
	if(a==bkg)
	{
		a++;
	}

//	if(a==0 && blb==NULL)
//	{
//		a++; /* try to avoid using black foreground codes */
//	}

	if(colour[a]>colour[best])
		{
			best=a;
		}
}
	if(best==bkg || colour[best]==0) /* || */
	{
		best=pbest;
	}

	if(best!=pbest && chars<39) /* chars==0) */
	{
		if(best==0 && nbf!=NULL && bkga!=NULL)
		{
			/* workaround for black foreground */
			best=bkg;
			bkg=0;
			a=1;
		}
		else
		{
			a=0;
		}

			if(format!=NULL)
			{
				data[count2]=esc;
				count2++;
			}
			data[count2]=(base+best);
			count2++;
			chars++;
			count=count+6;
			pbest=best;

		if(a==1)
		{
			/* workaround for black foreground - change bkg to black */
			if(format!=NULL)
			{
		            data[count2]=esc;
   		         count2++;
			}
        	data[count2]=bkgr;
			count2++;
			chars++;
			count=count+6;
		}


	}

	if(best==bkg)
	{
		best=bkg+1;
	}

				mosaic=32;
				if (screen[count]==best)
				{
					mosaic=mosaic+1;
				};

				if (screen[count+1]==best)
				{
					mosaic=mosaic+2;
				};

				if (screen[count+2]==best)
				{
					mosaic=mosaic+4;
				};

				if (screen[count+3]==best)
				{
					mosaic=mosaic+8;
				};

				if (screen[count+4]==best)
				{
					mosaic=mosaic+16;
				};

				if (screen[count+5]==best)
				{
					mosaic=mosaic+64;
				};

		if(mosaic==127 && bkga!=0)
		{
			bkg=best;

			if(format!=NULL)
			{
		            data[count2]=esc;
   		         count2++;
			}
 	        if(bkg!=0)
        	 {
    	        data[count2]=bkgc;
        	 }
 	        else
        	 {
         	data[count2]=bkgr;
    		     }


           /* count=count+6; */

		}
		else
		{
			data[count2]=mosaic;
		}
			count2++;
			chars++;


			if(chars==40)
			{
				chars=0;
				pbest=0;
				if(bkga) bkg=0;

			};

		};


        printf("Writing CEPT-3 file in ");
        if(format==NULL)
        {
        	printf(".bin/.vid format...\n");
        }
        else
        {
        	printf(".vtx format...\n");
        }

        result=fwrite(data,count2,1,fp2); /* 960 */

        if(result!=1)
        {
            printf("Error writing to %s\n",argv[2]);
  				fclose(fp1);
				fclose(fp2);

            exit(1);
        };

        printf("Finished.\n");

        fclose(fp1);
        fclose(fp2);

		exit(0);
    };
}

