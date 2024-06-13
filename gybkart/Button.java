import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

/**
 * Write a description of class Button here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public abstract class Button extends Actor
{
    /**
     * Act - do whatever the Button wants to do. This method is called whenever
     * the 'Act' or 'Run' button gets pressed in the environment.
     */
    protected GreenfootImage _image;

    public abstract void buttonFunc();
    public Button(GreenfootImage image){
        _image = image;
        this.setImage(_image);
    }
    public void act()
    {
        if(Greenfoot.mouseClicked(this)){
            buttonFunc();
        }
    }
}
