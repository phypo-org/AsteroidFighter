
#include <T3d/T3dCalcul.h>
#include <T3d/T3dColor.h>
#include <T3d/T3dPrimitiv.h>

#include <O3dObjBSpline.h>

#include <O3dObjProps.h>


/*---------------------------------------------------------------------*/
/*    O3dObjBezier::drawObj ...                                          */
/*---------------------------------------------------------------------*/

void
O3dObjBSpline::drawObj(O3dViewProps& pVProps, O3dObjProps* pObjProps)
{
	glDisable( GL_LIGHTING );

	pObjProps->setLinesProps();

	T3dColor::Red();
//	if( pVProps.isModeDraw( O3dViewProps::DRAW_POINT) )
	{
		glBegin( GL_POINTS);
		cData.doGlVertex3d();
		glEnd();
	}
	T3dColor::Green();

//	if( pVProps.isModeDraw( O3dViewProps::DRAW_POLE) )
	{
		glBegin( GL_POINTS );
		cPole.doGlVertex3d();
		glEnd();
	}

	int lMaille = pVProps.ViewPropsInt::get(MAILLE);
	DoPoint lDoPoint;

	glBegin( GL_LINE_STRIP );
	T3dPrimitiv::MakeBSplinePoint( lDoPoint, lMaille, cPole.getAll(), cPole.getNbPoint() );
	glEnd();

	glEnable( GL_LIGHTING );
}
/*---------------------------------------------------------------------*/
/*    O3dObjBSpline::makePoles ...                                    */
/*---------------------------------------------------------------------*/

void
O3dObjBSpline::makePoles()
{
	T3dPrimitiv::MakePoles( cData.getAll(), cData.getNbPoint(), cPole );
}


