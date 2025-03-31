document.addEventListener("DOMContentLoaded", function () {
  const checkbox = document.getElementById("confirmUpload");
  const simpleUploadSection = document.getElementById("simple-upload");
  const advancedUploadSection = document.getElementById("advanced-upload");

  // Toggle between simple and advanced upload modes
  checkbox.addEventListener("change", function () {
    if (checkbox.checked) {
      simpleUploadSection.classList.add("hidden");
      advancedUploadSection.classList.remove("hidden");
    } else {
      simpleUploadSection.classList.remove("hidden");
      advancedUploadSection.classList.add("hidden");
    }
  });


  const simpleUploadForm = document.getElementById("simpleUploadForm");
  simpleUploadForm.addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent the form from submitting normally
    // Here, you can add your upload logic (AJAX request, etc.)

    // After the request is sent successfully, reset the form
    simpleUploadForm.reset();
    alert("File uploaded successfully!");
  });

  // Handle advanced upload form submission
  const advancedUploadForm = document.getElementById("advancedUploadForm");
  advancedUploadForm.addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent the form from submitting normally
    // Here, you can add your upload logic (AJAX request, etc.)

    // After the request is sent successfully, reset the form
    advancedUploadForm.reset();
    alert("File created and uploaded successfully!");
  });

});
