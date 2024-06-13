import greenfoot.*;  // (World, Actor, GreenfootImage, Greenfoot and MouseInfo)

/**
 * Write a description of class Menu here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Menu extends World
{

    /**
     * Constructor for objects of class Menu.
     * 
     */
    public Menu()
    {    
        // Create a new world with 600x400 cells with a cell size of 1x1 pixels.
        super(256, 256, 1);
        this.showText("gybkart", 127, 60);
        ChangeWorld map1 = new ChangeWorld(new GreenfootImage("../images/button1.png"), new Map1());
        addObject(map1, 60, 130);
        ChangeWorld map2 = new ChangeWorld(new GreenfootImage("../images/button2.png"), new Map2());
        addObject(map2, 60, 165);
        Quit quit = new Quit(new GreenfootImage("../images/quit.png"));
        addObject(quit, 60, 200);
    }
}
