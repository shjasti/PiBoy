import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.ColorModel;
import java.awt.image.MemoryImageSource;

public class GPU {
	// Define a 2d array of 8x8 pixel tiles
	int [][][] tileset;
	
	// Define a boolean to say whether we're using the BG map or sprite map
	boolean bgMap;
	
	// VRAM
	byte vram[];
	
	// Define the array for the 4 colors
	byte[][] colors;
	
	// Define the width and height for the screen
	final int width = 160;
	final int height = 144;
	
	// Define the scanX and the scanY
	int scanX;
	int scanY;

	// Define the canvas
	byte[] canvas;
	
	// int to hold mode
	int mode;
	
	// int to hold modeClock
	int modeClock;
	
	// int to hold line
	int line;
	
	// Init the canvas
	public GPU() {
		// Init scanX, scanY
		scanX = 0;
		scanY = 0;
		
		// Set bgMap to true
		bgMap = true;
		
		// Define the colors
		byte gray1 = (byte) 0;
		byte gray2 = (byte) 85;
		byte gray3 = (byte) 171;
		byte gray4 = (byte) 255;
		
		// Init the canvas
		canvas = new byte[width * height * 4];
		
		// Init the color array
		colors = new byte[][]{
				{gray1, gray1, gray1, (byte) 255},
				{gray2, gray2, gray2, (byte) 255},
				{gray3, gray3, gray3, (byte) 255},
				{gray4, gray4, gray4, (byte) 255}
			};
		
		// Paint the roses white
		for (int i=0; i<width*height; i++)
			canvas[i] = (byte)255;
		
		// Init runtime ints
		mode = 2;
		modeClock = 0;
		line = 0;
	}
	
	// Step the GPU
	public void step()
	{
		// Sync the GPU clock to the CPU clock
		modeClock += Z80.r.t;
		
		switch (mode)
		{
			// Sprite memory read mode
			case 2:
				if (modeClock >= 80)
				{
					// Enter horizontal blank
					modeClock = 0;
					mode = 0;
				}
				
				break;
			
			// VRAM read mode
			case 3:
				if (modeClock >= 172)
				{
					// Enter horizontal blank
					modeClock = 0;
					mode = 0;
					
					// Write a scanline to the framebuffer
					// renderscan();
				}
				
				break;
			
			// Horizontal blank
			case 0:
				if (modeClock > 204)
				{
					// Reset the clock and increment the line
					modeClock = 0;
					line ++;
					
					// if we're at the last line, scan over to the top left corner
					if (line == 143)
					{
						mode = 1;
						
						//Convert to an image like so:
						MemoryImageSource source = new MemoryImageSource(width, height, this.toIntArray(), 0, width);
						Image image = Toolkit.getDefaultToolkit().createImage(source);
					}
				}
				
				break;
				
			// Vertical blank (10 lines)
		    case 1:
		        if(modeClock >= 456)
		        {
		        	modeClock = 0;
		        	line++;
		        }
		        if(line > 153)
			    {
			        // Restart scanning modes
		        	mode = 2;
		        	line = 0;
			    }
		        break;
		}
	}
	
	private void renderScan()
	{
		// VRAM offset for the tileset we're using
		int mapoffs = bgMap? 0x1C00: 0x1800;
		
		// Get the offset for the line of tiles to use
		mapoffs += ((line + scanY) & 255) >> 3;
		
		// Get the tile from the line
		int lineoffs = scanX >> 3;
		
		// Get the y value
		int y = (y + scanY) & 7;
		
		// Get the x value
		int x = scanX & 7;
		
		// Position on the canvas
		int canvasoffs = line * 160 * 4;
		
		// Get tile
		int tile = vram[mapoffs + lineoffs];
		
		// If we're using the Background tileset, offset the negatives
		if (bgMap && tile < 128)
			tile += 256;
		
		for (int i = 0; i < 160; i++)
		{
			// Get the color
			byte [] color = colors[tileset[tile][y][x]];
			
			screen.data[canvasoffs + 0] = color[0];
			screen.data[canvasoffs + 1] = color[1];
			screen.data[canvasoffs + 2] = color[2];
			screen.data[canvasoffs + 3] = color[3];
		}
		
	    // When this tile ends, read another
	    x++;
	    
	    if (x == 8)
	    {
	    	x = 0;
	    	lineoffs = (lineoffs + 1) & 31;
	    	tile = vram[mapoffs + lineoffs];
			if(bgMap && tile < 128) tile += 256;
	    }
	}
	
	// Reset tiles
	private void resetTileset()
	{
		// Init tileset
		tileset = new int[384][8][8];
		
		for (int tiles = 0; tiles < 384; tiles ++)
		{
			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					tileset[tiles][y][x] = 0;
				}
			}
		}
	}
	
	// Updates the tile set. Called by MMU.java after VRAM is changed
	public void updateTile(int addr)
	{
		// Get the base address for the tile row
		addr &= 0x1FFE;
		
		// Work out which tile and row was updated
		int tile = (addr >> 4) & 511;
		int y = (addr >> 1) & 7;
		
		// Init variable for x mask
		int xMask;
		for (int x = 0; x < 8; x++)
		{
			// Find the bit index for this pixel
			xMask = 1 << (7-x);
			
			// Update tile set
			tileset[tile][y][x] = ((vram[addr] & xMask) == 1? 1: 0) + ((vram[addr+1] & xMask) == 1? 2: 0);
		}
	}
	
	//  Private function to turn each pixel into an int
	private int[] toIntArray ()
	{
		// int pixel to hold each pixel
		int pixel = 0;
		
		// int array to return
		int[] intCanvas = new int[width * height];
		
		// Loop to convert 4 bytes into an int
		for (int i = 0; i < width * height * 4; i++)
		{
			pixel += canvas[i] << 8*(i%4);
			
			if (i%4 == 3)
			{
				intCanvas[(i+1)/4] = pixel;
				pixel = 0;
			}
		}
		
		// Return the array
		return intCanvas;
	}
}