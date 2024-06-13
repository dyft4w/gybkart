import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

/**
 * Write a description of class NormalMap1 here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class NormalMap1 extends NormalMap
{

    /**
     * Constructor for objects of class NormalMap1.
     * 
     */
    public NormalMap1()
    {
        super(new GreenfootImage("../images/map1_normal.png"));
    }
    @Override
    public MapInfo getModifs(double x, double y){
        int color = Util.getHexCode(_normal_map.getColorAt((int)x,(int)y));
        if(color==0x0000E0){
            return new MapInfo(new double[]{695,705}, true, true, 1, 0);
        }else{
            return super.getModifs(x,y);
        }
    }
}
