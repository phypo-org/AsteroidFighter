#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>


#include <FL/glut.H>

#include <O3d/O3dObjProps.h>
#include <O3d/O3dKamera.h>
#include <O3d/O3dObjText.h>


#include <Sprite3d/World.h>
#include <Sprite3d/ObjOdron.h>
#include <Sprite3d/ObjPart.h>
#include <Sprite3d/Sprite3d.h>
#include <Sprite3d/Utils3d.h>
#include <Sprite3d/Collision.h>
#include <Sprite3d/ObjText2d.h>
#include <Sprite3d/ObjText.h>
#include <Sprite3d/SceneManager.h>
#include <Sprite3d/SoundControler.h>

#include <MainMenu.h>
#include <Help.h>

#include <vector>


#include <Space/WorldGame.h>

////// #include <Invaders/WorldInvaders.h>

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/ini_parser.hpp>

int sStartNiveau = 0;

extern PSoundId sOceanSound;

//------------------------------------------------
// Enleve tout les espaces dans une chaines de caractere
char*
StrStrip( char* pStr )
{
  if( pStr == nullptr )	return nullptr;

  char *t = pStr;
  char *s = pStr;

  do{
    while( isspace( *s )  ) s++; 
    *t++ = *s;  
  }while( *s++ != '\0');
 
  return pStr;
}
//------------------------------------------------
bool
ReadIni( std::istream &pIn, std::map< std::string, std::string>& pConfig )
{
  char lLine[4096];

  std::string lCurrentSection = "None";


  //=============== read the stream ================
  while( pIn.getline( &lLine[0], sizeof(lLine)-1 ))
    {
      char* lCurrent = lLine;

      if( *lCurrent == '#')  // Comment ?
	continue;



      for( int i = (int)::strlen(lCurrent) -1 ; i >= 0; i-- )
        {
          if ( lCurrent[i] == '\r' )
            continue;

          lCurrent[i+1] = '\0';
          break;
        }


      while( isspace( *lCurrent )  ) lCurrent++;
      if( *lCurrent == '\0' )
	continue;

      // A section ?
      if ( *lCurrent == '[' )
        {
          char *lEnd = ::strchr( lCurrent, ']' );

          if( lEnd != nullptr )
	    *lEnd = '\0';

          lCurrentSection = ++lCurrent;

	  std::cout << "Current Section:" << lCurrentSection << std::endl;

	  continue;
        }

      char* lEgal = ::strchr(lCurrent, '=');
      if( lEgal == nullptr )
	{
	  std::cerr << "ReadIni error in section " << lCurrentSection << " >>>" << lLine << std::endl;
	  continue;
	}
      *lEgal  = '\0';

      char* lVal = StrStrip( lCurrent );
      lEgal++;

      if( lVal== nullptr || strlen( lVal ) <= 0
	  || strlen( lEgal ) <= 0 )
	{
	  std::cerr << "ReadIni error in section " << lCurrentSection << " >>>" << lLine << std::endl;
	  continue;
	}

      std::string lKey = lCurrentSection;
      lKey +='.';
      lKey +=lVal;

      std::cout << "ReadIni add values "  << lKey << '=' << lEgal << std::endl;
      pConfig[ lKey ] =  lEgal;
    }
  //===================================================
  return true;
}

//------------------------------------------------
bool
ReadFileIni( std::string & pName, std::map< std::string, std::string>& pConfig )
{
  if( pName.size() > 0 )
    {
      std::ifstream lFile( pName );
      return ReadIni( lFile, pConfig );
    }
  return false ;
}
//****************************************************
class GameWorldControler: public WorldControler{

  int          cSize;
  Double3      cMax;
  O3dObjProps* cGen;

public:
  GameWorldControler( int lTest, int pSize, int pWidth, int pHeight, bool pFullScreen)
    :WorldControler( pWidth, pHeight, pFullScreen ),
     cSize( pSize )		,
     cMax( 300, 200, 100 ),
     cGen(NULL)
  {


    Float4 mat1( 0.5, 0.5, 0.5, 1.0 );
    Float4 mat2( 0.0, 0.0, 0.0, 1.0 );

    O3dObjProps* lProps = new O3dObjProps();
    Float4 pr6 (0.3, 0.5, 0.9, 0.2);
    lProps->ObjPropsFloat4::set( MATERIAL, pr6);
    cGen = new O3dObjProps;

    cGen->ObjPropsFloat4::set( MATERIAL, mat1 );
    cGen->ObjPropsFloat4::set( EMISSION, mat2 );



    WorldGame::InitSprite();
    WorldControler::InitCallback();


    // Here, the game
    World *lWorld = new WorldGame( pSize, WorldControler::WC, new O3dKamera(), cMax,  cGen );
	       		

    WorldControler::WC->setGame( lWorld );




    // METTRE AILLEURS
    int W = glutGet(GLUT_WINDOW_WIDTH);
    int H = glutGet(GLUT_WINDOW_HEIGHT);
	
    //		std::cout << " call setSize W:" << W << " H:" << H << std::endl;
    WorldControler::WC->setSize( W, H );


		

    World  *lWorldHelp = new Help( pSize,  WorldControler::WC,new O3dKamera(), cMax,  cGen );
    lWorldHelp->initStart(sStartNiveau);
    WorldControler::WC->setHelpWorld( lWorldHelp );

    //	World  *lWorldMainMenu = new WorldInvaders( pSize,  WorldControler::WC,new O3dKamera(), cMax,  cGen );
    World  *lWorldMainMenu = new MainMenu( pSize,  WorldControler::WC,new O3dKamera(), cMax,  cGen );
    lWorldMainMenu->initStart( sStartNiveau );
    WorldControler::WC->setMainWorld( lWorldMainMenu );



  }



protected:
  void selectNewGame( unsigned char pKey, int pX, int pY )
  {
    World *lWorld = WorldControler::GetGameWorld();
    //    if( lWorld ) {
    //	    delete lWorld;
    //       }

    lWorld = new WorldGame( cSize, WorldControler::WC, new O3dKamera(), cMax,  cGen );
    WorldControler::WC->setGame( lWorld );
  }

  //------------------------------------------------

};
//****************************************************





void InitWorld( int pSize, int lTest)
{


  /*
    Menu3d* lMenu = new Menu3d();
    lMenu->addItem( "Go" );
    lMenu->addItem( "Help" );
    lMenu->pop();
    lMenu->affectToMouseButton(GLUT_LEFT_BUTTON);
  */
  /*
    ObjText* lOText = new ObjText( "Coucou" );
    Sprite3dObj* lSText = new Sprite3dObj( lOText );
    lSText->getTransf().TransfDouble3::get(POS)[ 0 ] = -120;
    Double3 lSpin3( randf( 50 ), randf( 50 ), randf( 50 ));

    lSText->SpriteDouble3::set( SPRITE_SPIN,  lSpin3 );
    lSText->getTransf().TransfDouble3::set( SCALE, Double3(0.1, 0.1, 0.1));

    lWorld->add( lSText );
  */


  // Sprite3d* lSp = TheShipsMaker->makeSpriteShip3();
  // lSp->SpriteDouble3::set( SPRITE_SPIN,  Double3( 50, 50, 50));
  // lWorld->add( lSp );
}

//------------------------------------------------
void
printHelp()
{
  std::cout << "StarFighter 0.5b" << std::endl;
  std::cout << "-F<number> frame/second ex: -F25" << std::endl;
  std::cout << "-S<number> size         ex: -S10" << std::endl;
}
//****************************************************
int main(int argc, char **argv)
{
#ifdef X_LINUX
  XInitThreads();
#endif // X_LINUX


  std::cout << " sizeof(int)=" << sizeof(int)
	    << " sizeof(long)=" << sizeof(long)
	    << " sizeof(double)=" << sizeof(double)
	    << " sizeof(float)=" << sizeof(float)
	    << " sizeof(long long)=" << sizeof(long long)
	    << " sizeof(long double)=" << sizeof(long double)
	    << " sizeof(void*)=" << sizeof(void*)
	    << std::endl;

  //	int lRate = 60;
  int lRate =60;
  int lTest = 0;
  int lSize = 10;
  char *lData=NULL;
  bool lFullScreen = true;

  for( int i =1; i< argc; i++)
    {
      char *pStr = argv[i];
      if( *pStr == '-' )
	switch( *(++pStr))
	  {
	  case 'G' :
	    WorldControler::sDebug =1;
	    break;

	  case 'f' :
	    lFullScreen = false;
	    break;

	  case 'F' :
	    {
	      lRate = atoi( ++pStr );
	      if( lRate > 1 && lRate < 101 )
		WorldControler::SetRate( lRate );
	      break;
	    }
	  case 'X':
	    {
	      lSize = atoi( ++pStr);
	      break;
	    }

	  case 'D':
	    lData=++pStr;
	    break;
	  case 'T':
	    lTest = atoi(++pStr);
	    break;

	  case 'M':
	    SoundControler::sMute = GL_TRUE;
	    break;

	  case 's':
	    SoundControler::sNoSound = GL_TRUE;
	    break;

	  case 'S':
	    SoundControler::sNoSound = GL_FALSE;
	    break;

	  case 'N':
	    sStartNiveau = atoi( ++pStr);
	    break;

	  case 'H':
	  case 'h':
	  default:
	    printHelp();
	    break;
	  }
    }

  //	std::cout << "fullscreen: " << lFullScreen << " Size:" << lSize << std::endl;

  /*
    slScheduler sched ( 8000 ) ;
    sched . setSafetyMargin ( 0.128f ) ;
    slSample *s2 = new slSample ( "Data/sound/zzap.wav" , & sched ) ;
    s2 -> adjustVolume ( 5.0f  ) ;
    sched . playSample ( s2 ) ;
    while ( SL_TRUE )
    {
    Sleep ( 1000 / 30 ) ;
    sched . update () ;
    }
  */


  //	SoundControler lSoundControler;

  WorldGame::sIniFile="AsteroidFighter.ini";
  ReadFileIni( WorldGame::sIniFile, WorldGame::sConfig);

  //=====================================
  Fl::use_high_res_GL(1);
  // Fl_Window window( 80*lSize+10, 60*lSize+10 );

  // window.show(argc,argv);
  // window.begin();
  Utils3d::Init( "AsteroidFighter 1.0", argc, argv, 80*lSize, 60*lSize);
	
  //  window.end();
 	
  //=====================================
	

  GameWorldControler lWControl( lTest, lSize, 80*lSize, 60*lSize, lFullScreen );
	

  WorldControler::SetRate( lRate );



  //	InitWorld(lSize, lTest);

  lWControl.mainWorld();


  if( lData )
    {
      std::string lDataSound( lData );
      lDataSound += "/SoundOgg";
      WorldGame::InitSound(lDataSound.c_str()); // Pour les sons notamment
    }
  else
    {	
      std::map<std::string, std::string>::iterator lIterator = World::sConfig.find("Paths.Sounds");
      if( lIterator == World::sConfig.end() )
	{
	  WorldGame::InitSound("SoundOgg"); // Pour les sons notamment
	}
      else 
	{
	  std::string lName = lIterator->second;
	  WorldGame::InitSound(lName.c_str());
	}
    }



  Utils3d::Loop();

  return 0;
}
//****************************************************





