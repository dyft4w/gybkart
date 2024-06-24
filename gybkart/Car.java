import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

import java.awt.image.BufferedImage;


/**
 * Write a description of class car here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Car extends Actor
{
    /**
     * Act - do whatever the car wants to do. This method is called whenever
     * the 'Act' or 'Run' button gets pressed in the environment.
     */
    private boolean _init;
    private boolean _lock;
    private long _time;

    private int _lap;
    private boolean _checkpoint;

    private double _real_x;
    private double _real_y;
    private double _rad;
    private double[] _last_legal;

    private double _wm;
    private double _am;
    private double _sm;
    private double _dm;

    private GameMap _current_map;
    private NormalMap _current_normal_map;

    private void init(){
        _time = System.currentTimeMillis();
        _last_legal = new double[2];
        _checkpoint=false;
        _init = true;
        _lap=1;


        GreenfootImage car = new GreenfootImage("../images/car.png");
        car.scale(car.getWidth()*Includes._SCALE,car.getHeight()*Includes._SCALE);
        this.setImage(car);


        _current_map = (GameMap) this.getWorld();
        if(_current_map.getClass()==Map1.class){
            _current_normal_map = (NormalMap) new NormalMap1();
        }
        if(_current_map.getClass()==Map2.class){
            _current_normal_map = (NormalMap) new NormalMap2();
        }
        if(_current_map.getClass()==Map3.class){
            _current_normal_map = (NormalMap) new NormalMap3();
        }
        setCoords(_current_map.getDefaultCoords()[0], _current_map.getDefaultCoords()[1]);
    }
    private void end(){
        if(!_lock) {
            _lock = true;
            // on cache la voiture
            setLocation(1000, 1000);
            // on render la map sans le texte par dessus
            _current_map.mapRender(_rad, getCoords()[0], getCoords()[1]);
            _current_map.drawEndScreen(System.currentTimeMillis() - _time);
        }
    }


    public double[] getCoords() {
        return new double[]{_real_x, _real_y};
    }
    public void setCoords(double x, double y){
        _real_x=x;
        _real_y=y;
    }

    public void mouvement(){
        if(Greenfoot.isKeyDown("w")){
            double[] vector = Util.calcVec(_rad);
            double[] old = getCoords();
            MapInfo info = _current_normal_map.getModifs(old[0],old[1]);
            double[] new_pos;

            if(info.isLegal()){
                _last_legal=old;
                if(info.getCoords()!=null){
                    new_pos=info.getCoords();
                }else{
                    if(info.getLapStatus()==1){
                        _checkpoint=true;
                    }
                    if(info.getLapStatus()==2 && _checkpoint){
                        _checkpoint=false;
                        if(_lap==3){
                            end();
                        }else{
                            _lap++;
                        }
                    }
                    new_pos = new double[]{old[0] - Curves.forward(_wm) * vector[0] * info.getCoeff(),
                            old[1] - Curves.forward(_wm) * vector[1] * info.getCoeff()};
                   if(info.getMomentum()){
                        _wm/=1.15;
                    }
                }
            }else{
                _wm=0;
                new_pos=_last_legal;
            }

            _wm++;
            setCoords(new_pos[0], new_pos[1]);
        }else{
            _wm /=1.15;
        }
        if(Greenfoot.isKeyDown("a")){
            _rad +=Curves.angle(_am);
            _am++;
        }else{
            _am =0;
        }
        if(Greenfoot.isKeyDown("s")){
            double[] vector = Util.calcVec(_rad);
            double[] old = getCoords();
            MapInfo info = _current_normal_map.getModifs(old[0],old[1]);
            double[] new_pos = new double[2];

            if(info.isLegal()){
                _last_legal=old;
                if(info.getCoords()!=null){
                    new_pos=info.getCoords();
                }else{
                    if(info.getLapStatus()==1){
                        _checkpoint=true;
                    }
                    if(info.getLapStatus()==2 && _checkpoint){
                        _checkpoint=false;
                        if(_lap==3){
                            end();
                        }else{
                            _lap++;
                        }
                    }
                    new_pos = new double[]{old[0] + Curves.backwards(_sm) * vector[0] * info.getCoeff(),
                            old[1] + Curves.backwards(_sm) * vector[1] * info.getCoeff()};
                    if(info.getMomentum()){
                        _sm/=1.15;
                    }
                }
            }else{
                _sm=0;
                new_pos=_last_legal;
            }

            _sm++;
            setCoords(new_pos[0], new_pos[1]);
        }else{
            _sm /=1.3;
        }
        if(Greenfoot.isKeyDown("d")){
            _rad -=Curves.angle(_dm);
            _dm++;
        }else{
            _dm =0;
        }
    }

    public void act() {
        // On ne peut pas utiliser le constructeur de Car puisque greenfoot n'est pas encore initialisé, alors voici un hack
        if(!_init) {
            init();
        }
        if(!_lock) {
            _current_map.mapRender(_rad, getCoords()[0], getCoords()[1]);
            _current_map.renderHUD(System.currentTimeMillis() - _time, _lap);
            mouvement();
        }
    }
}
