// The GBA memory emulator!
// Address Legend:
//		0x0000 - 0x0100 --- When BIOS is mapped, it is the BIOS, otherwise it's ROM0, cartridge ROM bank 1
//		0x0101 - 0x3FFF --- ROM0, cartridge ROM bank 1
// 		0x4000 - 0x7FFF --- ROM1, cartridge ROM additional banks, made available 1 by 1
//		0x8000 - 0x9FFF --- Graphics VRAM
//		0xA000 - 0xBFFF --- Extra external RAM if needed
//		0xC000 - 0xDFFF --- GBoy's internal 8k of RAM
//		0xE000 - 0xFDFF --- Shadow of internal RAM up until last 512 bytes
//		0xFE00 - 0xFE9F --- Sprite info, position, attributes etc.
//		0xFF00 - 0xFF7F --- I/O control values
//		0xFF80 - 0xFFFF --- Zero-mapped RAM, 128 bytes of high speed ram mapped starting at 0

// As of this version, the program can support ROMS up to 32kb large. Once we code in the ability to switch ROM banks, it'll be able to support infinitely-sized ROMs

import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;

public class MMU
{
    // Initialize boolean to check if BIOS is mapped
    public boolean biosMapped;

    // Initialize the memory places
    // BIOS memory
    public byte[] bios;
    // Cartridge ROM - ArrayList type used in order to ease the load(String filePath) function
    public ArrayList<Byte> rom;
    // Internal working RAM
    public byte[] wram;
    // External cartridge RAM
    public byte[] eram;
    //Fast zero-mapped RAM
    public byte[] zram;

    // Constructor
    public MMU()
    {
        biosMapped = true;
        rom = new ArrayList<Byte>();
    }

    // Read a byte
    public byte readByte (int addr)
    {
        // Check the largest digit of the address
        switch (addr & 0xF000)
        {
        	// Depending on the progression of the program, it's either in BIOS or ROM0
            case 0x0000:
            	// It's in BIOS
                if (addr < 0x0100)
                	return bios[addr];
                // It just crossed over to ROM0
                else if (z80.pc == 0x0100)
                	biosMapped = false;
                
                // It's in ROM0
                return rom.get(addr).byteValue();
            
            // It's in ROM0
            case 0x1000:
            case 0x2000:
            case 0x3000:
            	return rom.get(addr).byteValue();
            
            // It's in ROM1
            case 0x4000:
            case 0x5000:
            case 0x6000:
            case 0x7000:
            	return rom.get(addr).byteValue();
            	
            // It's in Graphics VRAM
            case 0x8000:
            case 0x9000:
            	return GPU.vram[addr & 0x1FFF];
            
            // It's in external cartridge RAM
            case 0xA000:
            case 0xB000:
            	return eram[addr & 0x1FFF];
            	
            // It's in internal working RAM
            case 0xC000:
            case 0xD000:
            	return wram[addr & 0x1FFF];
            	
            // It's in Shadow RAM
            case 0xE000:
            	return wram[addr & 0x1FFF];
            
            // It's in Shadow RAM, Sprite RAM, I/O control, or Zero-mapped RAM
            case 0xF000:
            	
            	// Figure out which one it's in
            	switch (addr & 0x0F00)
            	{
            		// It's in Shadow RAM
            		case 0x100: case 0x200: case 0x300: case 0x400: case 0x500:
            		case 0x600: case 0x700: case 0x800: case 0x900: case 0xA00:
            		case 0xB00: case 0xC00: case 0xD00:
            			return wram[addr & 0x1FFF];
            		
            		// It's either in Sprite memory or 0
            		case 0xE00:
            			// Check which one
            			if (addr < 0xFFA0)
            				// It's in sprite memory
            				return GPU.sprite[addr & 0xFF];
            			// It's null memory after Sprite
            			return 0;
            		
            		// It's either in I/O control or Zero-mapped RAM
            		case 0xF00:
            			if (addr < 0xFF80)
            			{
            				// Handle I/O
            				return 0;
            			}
            			
            			// It's in zero-mapped RAM
            			return zram[addr & 0x7F];
            	}
        }
    }
    
    // Read a word, return as a short
    public short readWord(int addr)
    {
    	// Return the two bytes in order as a short - changed from tutorial b/c error
    	return (short) (((short) readByte(addr)) << 8 + (short) readByte(addr+1));
    }
    
    // Writes a byte to VRAM
    public void writeByte (int addr, int val)
    {
    	// Check only the highest byte
    	switch (addr & 0xF000)
    	{
    		// Only do stuff for VRAM cases
    		case 0x8000:
    		case 0x9000:
    			GPU.vram[addr & 0x1FFF] = val;
    			GPU.updateTile(addr);
    			break;
    	}
    }
    
    // Load a rom into the rom array
    public void load(String filePath) throws IOException
    {
    	FileInputStream in = null;

        try {
            in = new FileInputStream(filePath);
            Byte c;

            while ((c = new Byte((byte) in.read())) != -1) {
                rom.add(c);
            }
        } finally {
            if (in != null)
                in.close();
        }
    }
}