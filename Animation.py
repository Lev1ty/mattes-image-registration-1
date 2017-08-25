#
# Script for generating images illustrating the movement of images and change
# in
# similarity metric during registration.
#
import SimpleITK as sitk

import matplotlib
matplotlib.use('agg')

import matplotlib.pyplot as plt
import numpy as np

from PIL import Image

import Tkinter as tk

import itk

# Paste the two given images together.  On the left will be image1 and on the
# right image2.
# image2 is also centered vertically in the combined image.
def write_combined_image(image1, image2, horizontal_space, file_name):
    combined_image = sitk.Image((image1.GetWidth() + image2.GetWidth() + horizontal_space,
                                max(image1.GetHeight(), image2.GetHeight())), 
                                image1.GetPixelID(), image1.GetNumberOfComponentsPerPixel())
    combined_image = sitk.Paste(combined_image, image1, image1.GetSize(), (0, 0), (0, 0))
    combined_image = sitk.Paste(combined_image, image2, image2.GetSize(), (0, 0), 
                                (image1.GetWidth() + horizontal_space, 
                                 int(round((combined_image.GetHeight() - image2.GetHeight()) / 2))))
    sitk.WriteImage(combined_image, file_name)
    print file_name


# Callback invoked when the StartEvent happens, sets up our new data.
def start_plot():
    global metric_values, multires_iterations, positions
    
    metric_values = []
    multires_iterations = []
    positions = []


# Callback invoked when the EndEvent happens, do cleanup of data and figure.
def end_plot():
    global metric_values, multires_iterations, positions
    

    del multires_iterations
    # Close figure, we don't want to get a duplicate of the plot latter on.
    plt.close()


# Callback invoked when the IterationEvent happens, update our data and
# save an image that includes a visualization of the registered images and
# the metric value plot.
def save_plot(registration_method, fixed, moving, transform, file_name_prefix):

    

    #
    # Plotting the similarity metric values, resolution changes are marked with
    # a blue star.
    #
    global metric_values, multires_iterations, positions


    
    metric_values.append(registration_method.GetMetricValue())  
    positions.append(registration_method.GetOptimizerPosition())
    # Plot the similarity metric values
    plt.plot(metric_values, 'r')
    plt.plot(multires_iterations, [metric_values[index] for index in multires_iterations], 'b*')
    plt.xlabel('Iteration Number',fontsize=12)
    plt.ylabel('Metric Value',fontsize=12)

    # Convert the plot to a SimpleITK image (works with the agg matplotlib
    # backend, doesn't work
    # with the default - the relevant method is canvas_tostring_rgb())
    plt.gcf().canvas.draw()    
    plot_data = np.fromstring(plt.gcf().canvas.tostring_rgb(), dtype=np.uint8, sep='')
    plot_data = plot_data.reshape(plt.gcf().canvas.get_width_height()[::-1] + (3,))
    plot_image = sitk.GetImageFromArray(plot_data, isVector=True)

    
    #
    # Extract the central axial slice from the two volumes, compose it using
    # the transformation
    # and alpha blend it.
    #
    alpha = 0.7
    
    central_index = int(round((fixed.GetSize())[2] / 2))

    moving_transformed = sitk.Resample(moving, fixed, transform, 
                                       sitk.sitkLinear, 0.0, 
                                       moving_image.GetPixelIDValue())
    comb = (1.0 - alpha) * fixed + alpha * moving_transformed
    rgb_comb = sitk.Cast(sitk.RescaleIntensity(comb), sitk.sitkUInt8)
    rgb_comb = sitk.Compose(comb, comb, comb)
    sitk.WriteImage(comb, file_name_prefix + format(len(metric_values), '03d') + '[Combined]' + '.mhd')
    print file_name_prefix + format(len(metric_values), '03d') + '[Combined]' + '.mhd'
    # Extract the central slice in xy and alpha blend them
    combined = (1.0 - alpha) * fixed[:,:,central_index] + \
               alpha * moving_transformed[:,:,central_index]

    # Assume the alpha blended images are isotropic and rescale intensity
    # Values so that they are in [0,255], convert the grayscale image to
    # color (r,g,b).
    combined_slices_image = sitk.Cast(sitk.RescaleIntensity(combined), sitk.sitkUInt8)
    combined_slices_image = sitk.Compose(combined_slices_image,
                                         combined_slices_image,
                                         combined_slices_image)
    #vector_double = sitk.VectorDouble(2)
    #vector_double[0] = moving_transformed.GetSpacing()[0]
    #vector_double[1] = moving_transformed.GetSpacing()[1]
    #combined_slices_image.SetSpacing(vector_double)
    #print combined_slices_image.GetSpacing()

    normalized_spacing = np.asarray(moving_transformed.GetSpacing(), dtype='float')
    normalized_spacing /= normalized_spacing.min()
    print normalized_spacing

    sitk.WriteImage(combined_slices_image, 
                         file_name_prefix + format(len(metric_values), '03d') + '[XY].png')
    print file_name_prefix + format(len(metric_values), '03d') + '[XY].png'

    img = Image.open(file_name_prefix + format(len(metric_values), '03d') + '[XY].png')
    w, h = img.size
    img = img.resize((int(w * normalized_spacing[0]), int(h * normalized_spacing[1])))
    img.save(file_name_prefix + format(len(metric_values), '03d') + '[XY].png')

    
    central_index = int(round((fixed.GetSize())[1] / 2))

    # Extract the central slice in xy and alpha blend them
    combined = (1.0 - alpha) * fixed[:,central_index,:] + \
               alpha * moving_transformed[:,central_index,:]

    # Assume the alpha blended images are isotropic and rescale intensity
    # Values so that they are in [0,255], convert the grayscale image to
    # color (r,g,b).
    combined_slices_image = sitk.Cast(sitk.RescaleIntensity(combined), sitk.sitkUInt8)
    combined_slices_image = sitk.Compose(combined_slices_image,
                                         combined_slices_image,
                                         combined_slices_image)


    sitk.WriteImage(combined_slices_image, 
                         file_name_prefix + format(len(metric_values), '03d') + '[XZ].png')
    print file_name_prefix + format(len(metric_values), '03d') + '[XZ].png'
    
    img = Image.open(file_name_prefix + format(len(metric_values), '03d') + '[XZ].png')
    w, h = img.size
    img = img.resize((int(w * normalized_spacing[0]), int(h * normalized_spacing[2])))
    img.save(file_name_prefix + format(len(metric_values), '03d') + '[XZ].png')
    
    central_index = int(round((fixed.GetSize())[0] / 2))

    # Extract the central slice in xy and alpha blend them
    combined = (1.0 - alpha) * fixed[central_index,:,:] + \
               alpha * moving_transformed[central_index,:,:]

    # Assume the alpha blended images are isotropic and rescale intensity
    # Values so that they are in [0,255], convert the grayscale image to
    # color (r,g,b).
    combined_slices_image = sitk.Cast(sitk.RescaleIntensity(combined), sitk.sitkUInt8)
    combined_slices_image = sitk.Compose(combined_slices_image,
                                         combined_slices_image,
                                         combined_slices_image)

    sitk.WriteImage(combined_slices_image, 
                         file_name_prefix + format(len(metric_values), '03d') + '[YZ].png')
    print file_name_prefix + format(len(metric_values), '03d') + '[YZ].png'
    
    img = Image.open(file_name_prefix + format(len(metric_values), '03d') + '[YZ].png')
    w, h = img.size
    img = img.resize((int(w * normalized_spacing[1]), int(h * normalized_spacing[2])))
    img.save(file_name_prefix + format(len(metric_values), '03d') + '[YZ].png')

    print metric_values
    print positions

    

    
# Callback invoked when the sitkMultiResolutionIterationEvent happens, update
# the index into the
# metric_values list.
def update_multires_iterations():
    global metric_values, multires_iterations, positions
    multires_iterations.append(len(metric_values))

if __name__ == '__main__':

    #gui = tk.Tk()
    #gui.mainloop()

    # Read the images
    fixed_image = sitk.ReadImage("C:/Users/Adam Yu/Desktop/CT/CT.mhd", sitk.sitkFloat64)
    moving_image = sitk.ReadImage("C:/MR (2).mhd", sitk.sitkFloat64)

    # permute axes
    """
    permute the axes of the moving image such that they correspond with the fixed image

    0 corresponds to column axes
    1 corresponds to row axes
    2 corresponds to slice axes
    """
    moving_image = sitk.PermuteAxes(moving_image, [0, 1, 2])

    # flip axes
    """
    flip the axes of the moving image such that they correspond with the fixed image

    each boolean corresponds to an axes
    """
    moving_image = sitk.Flip(moving_image, [False, False, False])

    # Initial alignment of the two volumes
    transform = sitk.CenteredTransformInitializer(fixed_image, #
                                                  moving_image, #
                                                  sitk.Euler3DTransform(), #
                                                  sitk.CenteredTransformInitializerFilter.GEOMETRY)#

    # Multi-resolution rigid registration using Mutual Information
    registration_method = sitk.ImageRegistrationMethod()#
    registration_method.SetMetricAsMattesMutualInformation(numberOfHistogramBins=50)#
    registration_method.SetMetricSamplingStrategy(registration_method.RANDOM)#
    registration_method.SetMetricSamplingPercentage(0.01)#
    registration_method.SetInterpolator(sitk.sitkLinear) # not in v4 c++
    global convergenceMinimumValueDef#
    convergenceMinimumValueDef = 1e-6#
    registration_method.SetOptimizerAsGradientDescent(learningRate=1.0, #
                                                      numberOfIterations=1000, #
                                                      convergenceMinimumValue=convergenceMinimumValueDef, #
                                                      convergenceWindowSize=10)#
    registration_method.SetOptimizerScalesFromPhysicalShift()#????????????????????????????????????????????????????

    # Setup for the multi-resolution framework.
    registration_method.SetShrinkFactorsPerLevel(shrinkFactors = [4,2,1])#
    registration_method.SetSmoothingSigmasPerLevel(smoothingSigmas=[2,1,0])#
    registration_method.SmoothingSigmasAreSpecifiedInPhysicalUnitsOn()#
    registration_method.SetInitialTransform(transform)

    prefix = 'C:/Users/Adam Yu/Desktop/Iterations2/'
    # Add all the callbacks responsible for ploting
    registration_method.AddCommand(sitk.sitkStartEvent, start_plot)
    registration_method.AddCommand(sitk.sitkEndEvent, end_plot)
    registration_method.AddCommand(sitk.sitkMultiResolutionIterationEvent, update_multires_iterations) 
    registration_method.AddCommand(sitk.sitkIterationEvent, lambda: save_plot(registration_method, fixed_image, moving_image, transform, prefix))

    registration_method.Execute(fixed_image, moving_image)#

    moving_transformed = sitk.Resample(moving_image, fixed_image, transform, sitk.sitkLinear, 0.0, moving_image.GetPixelID())#
    sitk.WriteImage(moving_transformed, prefix + 'FinalMovingTransformed.mhd')
    final_combined = 0.7 * moving_transformed + 0.3 * fixed_image
    final_combined = sitk.Cast(sitk.RescaleIntensity(final_combined), sitk.sitkUInt8)
    final_combined = sitk.Compose(final_combined,
                                         final_combined,
                                         final_combined)
    sitk.WriteImage(final_combined, prefix + 'FinalCombined.mhd')
    metric_values.append(registration_method.GetMetricValue())
    open(prefix + 'MetricValues.txt', 'w').write(str(metric_values))
    open(prefix + 'Positions.txt', 'w').write(str(positions))
    del metric_values
    del positions