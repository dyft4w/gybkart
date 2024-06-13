import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

import java.awt.*;
import java.awt.Color;
import java.awt.image.BufferedImage;


public abstract class GameMap extends World{
    protected final int[] _default_coords;
    protected final GreenfootImage _reference_image;
    protected GreenfootImage _base_img;
    protected Graphics2D _current_image;
    private final int _WIDTH=256;
    private final int _HEIGHT=256;

    public GameMap(int[] defaultCoords, GreenfootImage referenceImage)
    {
        super(256*Includes._SCALE, 256*Includes._SCALE, 1);

        this._default_coords = defaultCoords;
        _reference_image = referenceImage;
        _base_img = _reference_image;
        _current_image = (Graphics2D) _base_img.getAwtImage().getGraphics();
        this.setBackground(_base_img);
        Car car = new Car();
        addObject(car, 128* Includes._SCALE, 180* Includes._SCALE);
    }
    public int[] getDefaultCoords(){
        return _default_coords;
    }
    public void renderHUD(long time, int lap){
        _current_image.drawString("Temps: "+Util.formatTime(time), 0, 10);
        _current_image.drawString("Tour: "+lap+"/3", 0, 20);
    }
    public void mapRender(double rad, double x, double y){
        BufferedImage newImg = Util.rotateImageByDegrees(_reference_image.getAwtImage().getSubimage((int) (x - 128-_WIDTH/2+_WIDTH*2), (int) (y - 180-_HEIGHT/2+_HEIGHT*2), 512, 512), rad, 256, 308);
        _current_image.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
                RenderingHints.VALUE_INTERPOLATION_BILINEAR);
        _current_image.drawImage(newImg, 0, 0, _WIDTH* Includes._SCALE, _HEIGHT* Includes._SCALE, 0, 0, _WIDTH, _HEIGHT, null);
    }
    public void drawEndScreen(long time){
        _current_image.setColor(Color.WHITE);
        _current_image.fillRect(32*Includes._SCALE,32*Includes._SCALE,192*Includes._SCALE,192*Includes._SCALE);
        _current_image.setColor(Color.BLACK);
        _current_image.drawString("Vous avez gagné!", 40*Includes._SCALE,50*Includes._SCALE);
        _current_image.drawString("Temps: "+Util.formatTime(time), 40*Includes._SCALE,60*Includes._SCALE);
        ChangeWorld menu = new ChangeWorld(new GreenfootImage("../images/menu.png"), new Menu());
        addObject(menu, 40*Includes._SCALE+50, 200*Includes._SCALE);
    }
}
