package xmipp.viewer.particlepicker.training.model;

import java.util.ArrayList;

import xmipp.viewer.particlepicker.Family;
import xmipp.viewer.particlepicker.IJCommand;

public class ManualParticlePicker extends TrainingPicker {
	
	
	public ManualParticlePicker(String selfile, String outputdir, FamilyState mode) {

		this(selfile, outputdir, null, mode);

	}
	
	public ManualParticlePicker(String selfile, String outputdir, String fname, FamilyState mode) {

		super(selfile, outputdir, fname, mode);
		for (TrainingMicrograph m : micrographs)
			loadMicrographData(m);
		setUpdateTemplatesPending(true);
		updateTemplates(false);
		if(filters.isEmpty())//user just started manual mode and has no filter, I select gaussian blur by default, will be applied when window opens
		{
			filters.add(new IJCommand("Gaussian Blur...", "sigma=2"));
			persistFilters();
		}


	}




}
