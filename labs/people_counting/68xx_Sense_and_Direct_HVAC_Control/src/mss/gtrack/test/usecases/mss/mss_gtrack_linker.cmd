/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
	.gtrackLibSection {
		_gtrackLibStart = .;
		lib\libgtrack*.aer4f (.text)
		lib\libgtrack*.aer4f (.const)
		_gtrackLibEnd = .;
	} > PROG_RAM
	
	.MCPILogBuffer : {} > L3_RAM	
}
/*----------------------------------------------------------------------------*/

