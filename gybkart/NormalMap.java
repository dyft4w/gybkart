import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

/**
 * Write a description of class NormalMap here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public abstract class NormalMap
{

    /**
     * Constructor for objects of class NormalMap.
     * 
     */
    protected final GreenfootImage _normal_map;

    public NormalMap(GreenfootImage normalMap) {
        _normal_map = normalMap;
    }
    public MapInfo getModifs(double x, double y){
        /*
        * 0 : rien
        * 1 : impossible
        * 2 : ralentissement
        * 3 : checkpoint
        * 4 : ligne d'arrivée
        */
        MapInfo ret = new MapInfo();
        int color = Util.getHexCode(_normal_map.getColorAt((int)x,(int)y));
        if(color==0){
            ret.setLegal(false);
        }
        if(color==0x00E800){
            ret.setCoeff(0.4);
            ret.setMomentum(true);
        }
        if(color==0xed1c24){
            ret.setLapStatus(1);
        }
        if(color==0xa349a4){
            ret.setLapStatus(2);
        }
        return ret;
    }
}
