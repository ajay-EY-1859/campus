#include <gtk/gtk.h>
#include "core/services.h"

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *stack;
    GtkWidget *header_bar;
    
    GtkWidget *login_page;
    GtkWidget *register_page;
    GtkWidget *dashboard_page;
    GtkWidget *data_entry_page;
    GtkWidget *view_data_page;
    GtkWidget *profile_page;
    
    GtkWidget *login_user_entry;
    GtkWidget *login_mobile_entry;
    GtkWidget *login_password_entry;
    GtkWidget *login_status_label;
    
    GtkWidget *reg_name_entry;
    GtkWidget *reg_email_entry;
    GtkWidget *reg_mobile_entry;
    GtkWidget *reg_password_entry;
    GtkWidget *reg_confirm_entry;
    GtkWidget *reg_campus_combo;
    GtkWidget *reg_status_label;
    
    GtkWidget *dash_welcome_label;
    GtkWidget *dash_user_id_label;
    GtkWidget *dash_campus_label;
    GtkWidget *dash_email_label;
    GtkWidget *dash_mobile_label;
    GtkWidget *dash_data_view;
    
    GtkWidget *data_form_container;
    GtkWidget *data_status_label;
    
    GtkWidget *school_subject_entries[MAX_SUBJECTS];
    GtkWidget *school_marks_entries[MAX_SUBJECTS];
    GtkWidget *school_max_entries[MAX_SUBJECTS];
    int school_subject_count;
    
    GtkWidget *college_course_entry;
    GtkWidget *college_credits_entry;
    GtkWidget *college_gpa_entry;
    
    GtkWidget *hospital_patient_entry;
    GtkWidget *hospital_diagnosis_entry;
    GtkWidget *hospital_doctor_entry;
    GtkWidget *hospital_treatment_entry;
    
    GtkWidget *hostel_room_entry;
    GtkWidget *hostel_block_entry;
    GtkWidget *hostel_floor_entry;
    GtkWidget *hostel_type_entry;
    GtkWidget *hostel_rent_entry;
    
    GtkWidget *view_data_content;
    
    Profile current_user;
    int is_logged_in;
    
    SchoolData school_data;
    CollegeData college_data;
    HospitalData hospital_data;
    HostelData hostel_data;
} AppData;

static AppData *app_data = NULL;

static void show_message(GtkWidget *parent, const char *title, const char *message, GtkMessageType type) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", message);
    gtk_alert_dialog_show(dialog, GTK_WINDOW(parent));
    g_object_unref(dialog);
}

static void switch_to_page(const char *page_name) {
    gtk_stack_set_visible_child_name(GTK_STACK(app_data->stack), page_name);
    
    if (strcmp(page_name, "login") == 0 || strcmp(page_name, "register") == 0) {
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar), 
            gtk_label_new(APP_NAME));
    } else if (strcmp(page_name, "dashboard") == 0) {
        char title[256];
        snprintf(title, sizeof(title), "%s - %s Dashboard", APP_NAME, 
                 get_campus_name(app_data->current_user.campusType));
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar),
            gtk_label_new(title));
    } else if (strcmp(page_name, "data_entry") == 0) {
        char title[256];
        snprintf(title, sizeof(title), "%s - Add %s Data", APP_NAME, 
                 get_campus_name(app_data->current_user.campusType));
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar),
            gtk_label_new(title));
    } else if (strcmp(page_name, "view_data") == 0) {
        char title[256];
        snprintf(title, sizeof(title), "%s - View %s Data", APP_NAME, 
                 get_campus_name(app_data->current_user.campusType));
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar),
            gtk_label_new(title));
    } else if (strcmp(page_name, "profile") == 0) {
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar),
            gtk_label_new("User Profile"));
    }
}

static void update_dashboard(void) {
    char buf[512];
    
    snprintf(buf, sizeof(buf), "Welcome, %s!", app_data->current_user.name);
    gtk_label_set_text(GTK_LABEL(app_data->dash_welcome_label), buf);
    
    snprintf(buf, sizeof(buf), "User ID: %s", app_data->current_user.userID);
    gtk_label_set_text(GTK_LABEL(app_data->dash_user_id_label), buf);
    
    snprintf(buf, sizeof(buf), "Campus: %s", get_campus_name(app_data->current_user.campusType));
    gtk_label_set_text(GTK_LABEL(app_data->dash_campus_label), buf);
    
    snprintf(buf, sizeof(buf), "Email: %s", app_data->current_user.email);
    gtk_label_set_text(GTK_LABEL(app_data->dash_email_label), buf);
    
    snprintf(buf, sizeof(buf), "Mobile: %s", app_data->current_user.mobile);
    gtk_label_set_text(GTK_LABEL(app_data->dash_mobile_label), buf);
}

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    const char *userID = gtk_editable_get_text(GTK_EDITABLE(app_data->login_user_entry));
    const char *mobile = gtk_editable_get_text(GTK_EDITABLE(app_data->login_mobile_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->login_password_entry));
    
    if (strlen(userID) == 0 || strlen(mobile) == 0 || strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "Please fill all fields");
        return;
    }
    
    ErrorCode result = authenticate_user(userID, mobile, password);
    
    if (result == SUCCESS) {
        get_user_by_id(userID, &app_data->current_user);
        app_data->is_logged_in = 1;
        update_dashboard();
        switch_to_page("dashboard");
        
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_user_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_mobile_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_password_entry), "");
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), "");
    } else if (result == ERROR_NOT_FOUND) {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "User not found. Please register first.");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "Invalid credentials. Please try again.");
    }
}

static void on_register_clicked(GtkButton *button, gpointer user_data) {
    const char *name = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_name_entry));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_email_entry));
    const char *mobile = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_mobile_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_password_entry));
    const char *confirm = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_confirm_entry));
    int campus_idx = gtk_drop_down_get_selected(GTK_DROP_DOWN(app_data->reg_campus_combo));
    
    if (strlen(name) == 0 || strlen(email) == 0 || strlen(mobile) == 0 || 
        strlen(password) == 0 || strlen(confirm) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Please fill all fields");
        return;
    }
    
    if (!validate_email(email)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Invalid email format");
        return;
    }
    
    if (!validate_mobile(mobile)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Mobile must be 10 digits");
        return;
    }
    
    if (!validate_password(password)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Password must be 8+ chars with upper, lower, digit");
        return;
    }
    
    if (strcmp(password, confirm) != 0) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Passwords do not match");
        return;
    }
    
    CampusType campusType = (CampusType)(campus_idx + 1);
    
    Profile newUser = {0};
    generate_user_id(newUser.userID, campusType);
    strncpy(newUser.name, name, MAX_LEN - 1);
    strncpy(newUser.email, email, MAX_LEN - 1);
    strncpy(newUser.mobile, mobile, MAX_LEN - 1);
    hash_password(password, newUser.passwordHash);
    newUser.campusType = campusType;
    strncpy(newUser.campusName, get_campus_name(campusType), MAX_LEN - 1);
    newUser.createdAt = time(NULL);
    
    ErrorCode result = create_user(&newUser);
    
    if (result == SUCCESS) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Registration successful! Your User ID: %s", newUser.userID);
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), msg);
        
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_name_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_email_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_mobile_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_password_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_confirm_entry), "");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Registration failed. Please try again.");
    }
}

static void on_logout_clicked(GtkButton *button, gpointer user_data) {
    log_activity(app_data->current_user.userID, "LOGOUT", "User logged out");
    memset(&app_data->current_user, 0, sizeof(Profile));
    app_data->is_logged_in = 0;
    switch_to_page("login");
}

static void on_goto_register(GtkButton *button, gpointer user_data) {
    switch_to_page("register");
}

static void on_goto_login(GtkButton *button, gpointer user_data) {
    switch_to_page("login");
}

static void on_back_to_dashboard(GtkButton *button, gpointer user_data) {
    switch_to_page("dashboard");
}

static void on_save_school_data(GtkButton *button, gpointer user_data) {
    memset(&app_data->school_data, 0, sizeof(SchoolData));
    app_data->school_data.numSubjects = 0;
    
    for (int i = 0; i < MAX_SUBJECTS && app_data->school_subject_entries[i]; i++) {
        const char *subject = gtk_editable_get_text(GTK_EDITABLE(app_data->school_subject_entries[i]));
        const char *marks_str = gtk_editable_get_text(GTK_EDITABLE(app_data->school_marks_entries[i]));
        const char *max_str = gtk_editable_get_text(GTK_EDITABLE(app_data->school_max_entries[i]));
        
        if (strlen(subject) > 0 && strlen(marks_str) > 0 && strlen(max_str) > 0) {
            strncpy(app_data->school_data.subjects[app_data->school_data.numSubjects].subjectName, 
                    subject, MAX_LEN - 1);
            app_data->school_data.subjects[app_data->school_data.numSubjects].marks = atoi(marks_str);
            app_data->school_data.subjects[app_data->school_data.numSubjects].maxMarks = atoi(max_str);
            app_data->school_data.numSubjects++;
        }
    }
    
    if (app_data->school_data.numSubjects == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Please enter at least one subject");
        return;
    }
    
    calculate_school_grade(&app_data->school_data);
    
    ErrorCode result = save_campus_data(app_data->current_user.userID, CAMPUS_SCHOOL, &app_data->school_data);
    
    if (result == SUCCESS) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Data saved! Grade: %s (%.1f%%)", 
                 app_data->school_data.grade, app_data->school_data.percentage);
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), msg);
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Failed to save data");
    }
}

static void on_save_college_data(GtkButton *button, gpointer user_data) {
    const char *course = gtk_editable_get_text(GTK_EDITABLE(app_data->college_course_entry));
    const char *credits_str = gtk_editable_get_text(GTK_EDITABLE(app_data->college_credits_entry));
    const char *gpa_str = gtk_editable_get_text(GTK_EDITABLE(app_data->college_gpa_entry));
    
    if (strlen(course) == 0 || strlen(credits_str) == 0 || strlen(gpa_str) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Please fill all fields");
        return;
    }
    
    memset(&app_data->college_data, 0, sizeof(CollegeData));
    strncpy(app_data->college_data.courseName, course, MAX_LEN - 1);
    app_data->college_data.credits = atoi(credits_str);
    app_data->college_data.gpa = atof(gpa_str);
    calculate_college_cgpa(&app_data->college_data);
    
    ErrorCode result = save_campus_data(app_data->current_user.userID, CAMPUS_COLLEGE, &app_data->college_data);
    
    if (result == SUCCESS) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Data saved! CGPA: %.2f", app_data->college_data.cgpa);
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), msg);
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Failed to save data");
    }
}

static void on_save_hospital_data(GtkButton *button, gpointer user_data) {
    const char *patient = gtk_editable_get_text(GTK_EDITABLE(app_data->hospital_patient_entry));
    const char *diagnosis = gtk_editable_get_text(GTK_EDITABLE(app_data->hospital_diagnosis_entry));
    const char *doctor = gtk_editable_get_text(GTK_EDITABLE(app_data->hospital_doctor_entry));
    const char *treatment = gtk_editable_get_text(GTK_EDITABLE(app_data->hospital_treatment_entry));
    
    if (strlen(patient) == 0 || strlen(diagnosis) == 0 || strlen(doctor) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Please fill required fields");
        return;
    }
    
    memset(&app_data->hospital_data, 0, sizeof(HospitalData));
    strncpy(app_data->hospital_data.patientID, patient, MAX_LEN - 1);
    strncpy(app_data->hospital_data.diagnosis, diagnosis, MAX_LEN - 1);
    strncpy(app_data->hospital_data.doctorName, doctor, MAX_LEN - 1);
    strncpy(app_data->hospital_data.treatment, treatment, MAX_LEN - 1);
    app_data->hospital_data.admissionDate = time(NULL);
    
    ErrorCode result = save_campus_data(app_data->current_user.userID, CAMPUS_HOSPITAL, &app_data->hospital_data);
    
    if (result == SUCCESS) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Hospital data saved successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Failed to save data");
    }
}

static void on_save_hostel_data(GtkButton *button, gpointer user_data) {
    const char *room = gtk_editable_get_text(GTK_EDITABLE(app_data->hostel_room_entry));
    const char *block = gtk_editable_get_text(GTK_EDITABLE(app_data->hostel_block_entry));
    const char *floor_str = gtk_editable_get_text(GTK_EDITABLE(app_data->hostel_floor_entry));
    const char *type = gtk_editable_get_text(GTK_EDITABLE(app_data->hostel_type_entry));
    const char *rent_str = gtk_editable_get_text(GTK_EDITABLE(app_data->hostel_rent_entry));
    
    if (strlen(room) == 0 || strlen(block) == 0 || strlen(floor_str) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Please fill required fields");
        return;
    }
    
    memset(&app_data->hostel_data, 0, sizeof(HostelData));
    strncpy(app_data->hostel_data.roomNumber, room, MAX_LEN - 1);
    strncpy(app_data->hostel_data.blockName, block, MAX_LEN - 1);
    app_data->hostel_data.floorNumber = atoi(floor_str);
    strncpy(app_data->hostel_data.roomType, type, MAX_LEN - 1);
    app_data->hostel_data.monthlyRent = atof(rent_str);
    
    ErrorCode result = save_campus_data(app_data->current_user.userID, CAMPUS_HOSTEL, &app_data->hostel_data);
    
    if (result == SUCCESS) {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Hostel data saved successfully!");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "Failed to save data");
    }
}

static GtkWidget* create_school_form(void) {
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span weight='bold'>Enter Subject Data</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), title);
    
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);
    
    GtkWidget *hdr_subject = gtk_label_new("Subject");
    GtkWidget *hdr_marks = gtk_label_new("Marks");
    GtkWidget *hdr_max = gtk_label_new("Max Marks");
    gtk_widget_add_css_class(hdr_subject, "dim-label");
    gtk_widget_add_css_class(hdr_marks, "dim-label");
    gtk_widget_add_css_class(hdr_max, "dim-label");
    gtk_grid_attach(GTK_GRID(grid), hdr_subject, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), hdr_marks, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), hdr_max, 2, 0, 1, 1);
    
    for (int i = 0; i < 5; i++) {
        app_data->school_subject_entries[i] = gtk_entry_new();
        gtk_widget_set_size_request(app_data->school_subject_entries[i], 200, -1);
        gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->school_subject_entries[i]), "Subject name");
        gtk_grid_attach(GTK_GRID(grid), app_data->school_subject_entries[i], 0, i + 1, 1, 1);
        
        app_data->school_marks_entries[i] = gtk_entry_new();
        gtk_widget_set_size_request(app_data->school_marks_entries[i], 80, -1);
        gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->school_marks_entries[i]), "0");
        gtk_grid_attach(GTK_GRID(grid), app_data->school_marks_entries[i], 1, i + 1, 1, 1);
        
        app_data->school_max_entries[i] = gtk_entry_new();
        gtk_widget_set_size_request(app_data->school_max_entries[i], 80, -1);
        gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->school_max_entries[i]), "100");
        gtk_grid_attach(GTK_GRID(grid), app_data->school_max_entries[i], 2, i + 1, 1, 1);
    }
    
    gtk_box_append(GTK_BOX(form), grid);
    
    GtkWidget *save_btn = gtk_button_new_with_label("Save School Data");
    gtk_widget_add_css_class(save_btn, "suggested-action");
    gtk_widget_set_margin_top(save_btn, 16);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_school_data), NULL);
    gtk_box_append(GTK_BOX(form), save_btn);
    
    return form;
}

static GtkWidget* create_college_form(void) {
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(form, 400, -1);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span weight='bold'>Enter College Data</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), title);
    
    GtkWidget *course_label = gtk_label_new("Course Name");
    gtk_widget_set_halign(course_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), course_label);
    app_data->college_course_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->college_course_entry), "Enter course name");
    gtk_box_append(GTK_BOX(form), app_data->college_course_entry);
    
    GtkWidget *credits_label = gtk_label_new("Total Credits");
    gtk_widget_set_halign(credits_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), credits_label);
    app_data->college_credits_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->college_credits_entry), "Enter credits");
    gtk_box_append(GTK_BOX(form), app_data->college_credits_entry);
    
    GtkWidget *gpa_label = gtk_label_new("GPA (0.0 - 4.0)");
    gtk_widget_set_halign(gpa_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), gpa_label);
    app_data->college_gpa_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->college_gpa_entry), "Enter GPA");
    gtk_box_append(GTK_BOX(form), app_data->college_gpa_entry);
    
    GtkWidget *save_btn = gtk_button_new_with_label("Save College Data");
    gtk_widget_add_css_class(save_btn, "suggested-action");
    gtk_widget_set_margin_top(save_btn, 16);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_college_data), NULL);
    gtk_box_append(GTK_BOX(form), save_btn);
    
    return form;
}

static GtkWidget* create_hospital_form(void) {
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(form, 400, -1);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span weight='bold'>Enter Hospital Data</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), title);
    
    GtkWidget *patient_label = gtk_label_new("Patient ID");
    gtk_widget_set_halign(patient_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), patient_label);
    app_data->hospital_patient_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hospital_patient_entry), "Enter patient ID");
    gtk_box_append(GTK_BOX(form), app_data->hospital_patient_entry);
    
    GtkWidget *diagnosis_label = gtk_label_new("Diagnosis");
    gtk_widget_set_halign(diagnosis_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), diagnosis_label);
    app_data->hospital_diagnosis_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hospital_diagnosis_entry), "Enter diagnosis");
    gtk_box_append(GTK_BOX(form), app_data->hospital_diagnosis_entry);
    
    GtkWidget *doctor_label = gtk_label_new("Doctor Name");
    gtk_widget_set_halign(doctor_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), doctor_label);
    app_data->hospital_doctor_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hospital_doctor_entry), "Enter doctor name");
    gtk_box_append(GTK_BOX(form), app_data->hospital_doctor_entry);
    
    GtkWidget *treatment_label = gtk_label_new("Treatment");
    gtk_widget_set_halign(treatment_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), treatment_label);
    app_data->hospital_treatment_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hospital_treatment_entry), "Enter treatment");
    gtk_box_append(GTK_BOX(form), app_data->hospital_treatment_entry);
    
    GtkWidget *save_btn = gtk_button_new_with_label("Save Hospital Data");
    gtk_widget_add_css_class(save_btn, "suggested-action");
    gtk_widget_set_margin_top(save_btn, 16);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_hospital_data), NULL);
    gtk_box_append(GTK_BOX(form), save_btn);
    
    return form;
}

static GtkWidget* create_hostel_form(void) {
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(form, 400, -1);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span weight='bold'>Enter Hostel Data</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), title);
    
    GtkWidget *room_label = gtk_label_new("Room Number");
    gtk_widget_set_halign(room_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), room_label);
    app_data->hostel_room_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hostel_room_entry), "Enter room number");
    gtk_box_append(GTK_BOX(form), app_data->hostel_room_entry);
    
    GtkWidget *block_label = gtk_label_new("Block Name");
    gtk_widget_set_halign(block_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), block_label);
    app_data->hostel_block_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hostel_block_entry), "Enter block name");
    gtk_box_append(GTK_BOX(form), app_data->hostel_block_entry);
    
    GtkWidget *floor_label = gtk_label_new("Floor Number");
    gtk_widget_set_halign(floor_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), floor_label);
    app_data->hostel_floor_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hostel_floor_entry), "Enter floor number");
    gtk_box_append(GTK_BOX(form), app_data->hostel_floor_entry);
    
    GtkWidget *type_label = gtk_label_new("Room Type");
    gtk_widget_set_halign(type_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), type_label);
    app_data->hostel_type_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hostel_type_entry), "Single/Double/Triple");
    gtk_box_append(GTK_BOX(form), app_data->hostel_type_entry);
    
    GtkWidget *rent_label = gtk_label_new("Monthly Rent");
    gtk_widget_set_halign(rent_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), rent_label);
    app_data->hostel_rent_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->hostel_rent_entry), "Enter rent amount");
    gtk_box_append(GTK_BOX(form), app_data->hostel_rent_entry);
    
    GtkWidget *save_btn = gtk_button_new_with_label("Save Hostel Data");
    gtk_widget_add_css_class(save_btn, "suggested-action");
    gtk_widget_set_margin_top(save_btn, 16);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_hostel_data), NULL);
    gtk_box_append(GTK_BOX(form), save_btn);
    
    return form;
}

static void update_data_entry_form(void) {
    GtkWidget *child = gtk_widget_get_first_child(app_data->data_form_container);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(app_data->data_form_container), child);
        child = next;
    }
    
    GtkWidget *form = NULL;
    switch (app_data->current_user.campusType) {
        case CAMPUS_SCHOOL:
            form = create_school_form();
            break;
        case CAMPUS_COLLEGE:
            form = create_college_form();
            break;
        case CAMPUS_HOSPITAL:
            form = create_hospital_form();
            break;
        case CAMPUS_HOSTEL:
            form = create_hostel_form();
            break;
        default:
            form = gtk_label_new("Unknown campus type");
            break;
    }
    
    gtk_box_append(GTK_BOX(app_data->data_form_container), form);
}

static void on_add_data_clicked(GtkButton *button, gpointer user_data) {
    update_data_entry_form();
    gtk_label_set_text(GTK_LABEL(app_data->data_status_label), "");
    switch_to_page("data_entry");
}

static void update_view_data_content(void) {
    GtkWidget *child = gtk_widget_get_first_child(app_data->view_data_content);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(app_data->view_data_content), child);
        child = next;
    }
    
    char buf[1024];
    GtkWidget *info;
    
    switch (app_data->current_user.campusType) {
        case CAMPUS_SCHOOL: {
            SchoolData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_SCHOOL, &data) == SUCCESS) {
                snprintf(buf, sizeof(buf), "<span weight='bold'>School Report Card</span>");
                info = gtk_label_new(NULL);
                gtk_label_set_markup(GTK_LABEL(info), buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
                
                for (int i = 0; i < data.numSubjects; i++) {
                    snprintf(buf, sizeof(buf), "%s: %d / %d", 
                             data.subjects[i].subjectName, 
                             data.subjects[i].marks, 
                             data.subjects[i].maxMarks);
                    info = gtk_label_new(buf);
                    gtk_widget_set_halign(info, GTK_ALIGN_START);
                    gtk_box_append(GTK_BOX(app_data->view_data_content), info);
                }
                
                snprintf(buf, sizeof(buf), "\nPercentage: %.1f%%\nGrade: %s", 
                         data.percentage, data.grade);
                info = gtk_label_new(buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            } else {
                info = gtk_label_new("No school data found. Please add data first.");
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            }
            break;
        }
        case CAMPUS_COLLEGE: {
            CollegeData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_COLLEGE, &data) == SUCCESS) {
                snprintf(buf, sizeof(buf), "<span weight='bold'>College Record</span>");
                info = gtk_label_new(NULL);
                gtk_label_set_markup(GTK_LABEL(info), buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
                
                snprintf(buf, sizeof(buf), "Course: %s\nCredits: %d\nGPA: %.2f\nCGPA: %.2f",
                         data.courseName, data.credits, data.gpa, data.cgpa);
                info = gtk_label_new(buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            } else {
                info = gtk_label_new("No college data found. Please add data first.");
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            }
            break;
        }
        case CAMPUS_HOSPITAL: {
            HospitalData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_HOSPITAL, &data) == SUCCESS) {
                snprintf(buf, sizeof(buf), "<span weight='bold'>Hospital Record</span>");
                info = gtk_label_new(NULL);
                gtk_label_set_markup(GTK_LABEL(info), buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
                
                char timeStr[64];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", localtime(&data.admissionDate));
                snprintf(buf, sizeof(buf), "Patient ID: %s\nDiagnosis: %s\nDoctor: %s\nTreatment: %s\nAdmission Date: %s",
                         data.patientID, data.diagnosis, data.doctorName, data.treatment, timeStr);
                info = gtk_label_new(buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            } else {
                info = gtk_label_new("No hospital data found. Please add data first.");
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            }
            break;
        }
        case CAMPUS_HOSTEL: {
            HostelData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_HOSTEL, &data) == SUCCESS) {
                snprintf(buf, sizeof(buf), "<span weight='bold'>Hostel Record</span>");
                info = gtk_label_new(NULL);
                gtk_label_set_markup(GTK_LABEL(info), buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
                
                snprintf(buf, sizeof(buf), "Room: %s\nBlock: %s\nFloor: %d\nType: %s\nMonthly Rent: %.2f",
                         data.roomNumber, data.blockName, data.floorNumber, data.roomType, data.monthlyRent);
                info = gtk_label_new(buf);
                gtk_widget_set_halign(info, GTK_ALIGN_START);
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            } else {
                info = gtk_label_new("No hostel data found. Please add data first.");
                gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            }
            break;
        }
        default:
            info = gtk_label_new("Unknown campus type");
            gtk_box_append(GTK_BOX(app_data->view_data_content), info);
            break;
    }
}

static void on_view_data_clicked(GtkButton *button, gpointer user_data) {
    update_view_data_content();
    switch_to_page("view_data");
}

static void on_view_profile_clicked(GtkButton *button, gpointer user_data) {
    show_message(app_data->window, "Profile", "Profile details are shown on the dashboard.", GTK_MESSAGE_INFO);
}

static void on_export_pdf_clicked(GtkButton *button, gpointer user_data) {
    char filename[256];
    snprintf(filename, sizeof(filename), "data/%s_report.txt", app_data->current_user.userID);
    
    FILE *f = fopen(filename, "w");
    if (!f) {
        show_message(app_data->window, "Export", "Failed to create export file.", GTK_MESSAGE_ERROR);
        return;
    }
    
    fprintf(f, "=== Campus Management System Report ===\n\n");
    fprintf(f, "User: %s\n", app_data->current_user.name);
    fprintf(f, "User ID: %s\n", app_data->current_user.userID);
    fprintf(f, "Campus: %s\n", get_campus_name(app_data->current_user.campusType));
    fprintf(f, "Email: %s\n", app_data->current_user.email);
    fprintf(f, "Mobile: %s\n\n", app_data->current_user.mobile);
    
    switch (app_data->current_user.campusType) {
        case CAMPUS_SCHOOL: {
            SchoolData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_SCHOOL, &data) == SUCCESS) {
                fprintf(f, "=== School Report Card ===\n");
                for (int i = 0; i < data.numSubjects; i++) {
                    fprintf(f, "%s: %d / %d\n", data.subjects[i].subjectName, 
                            data.subjects[i].marks, data.subjects[i].maxMarks);
                }
                fprintf(f, "\nPercentage: %.1f%%\n", data.percentage);
                fprintf(f, "Grade: %s\n", data.grade);
            }
            break;
        }
        case CAMPUS_COLLEGE: {
            CollegeData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_COLLEGE, &data) == SUCCESS) {
                fprintf(f, "=== College Record ===\n");
                fprintf(f, "Course: %s\n", data.courseName);
                fprintf(f, "Credits: %d\n", data.credits);
                fprintf(f, "GPA: %.2f\n", data.gpa);
                fprintf(f, "CGPA: %.2f\n", data.cgpa);
            }
            break;
        }
        case CAMPUS_HOSPITAL: {
            HospitalData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_HOSPITAL, &data) == SUCCESS) {
                fprintf(f, "=== Hospital Record ===\n");
                fprintf(f, "Patient ID: %s\n", data.patientID);
                fprintf(f, "Diagnosis: %s\n", data.diagnosis);
                fprintf(f, "Doctor: %s\n", data.doctorName);
                fprintf(f, "Treatment: %s\n", data.treatment);
            }
            break;
        }
        case CAMPUS_HOSTEL: {
            HostelData data;
            if (load_campus_data(app_data->current_user.userID, CAMPUS_HOSTEL, &data) == SUCCESS) {
                fprintf(f, "=== Hostel Record ===\n");
                fprintf(f, "Room: %s\n", data.roomNumber);
                fprintf(f, "Block: %s\n", data.blockName);
                fprintf(f, "Floor: %d\n", data.floorNumber);
                fprintf(f, "Type: %s\n", data.roomType);
                fprintf(f, "Monthly Rent: %.2f\n", data.monthlyRent);
            }
            break;
        }
        default:
            break;
    }
    
    time_t now = time(NULL);
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(f, "\n\nGenerated: %s\n", timeStr);
    
    fclose(f);
    
    log_activity(app_data->current_user.userID, "EXPORT", "Report exported");
    
    char msg[512];
    snprintf(msg, sizeof(msg), "Report exported to: %s", filename);
    show_message(app_data->window, "Export", msg, GTK_MESSAGE_INFO);
}

static void on_change_password_clicked(GtkButton *button, gpointer user_data) {
    show_message(app_data->window, "Change Password", 
                 "Password change feature coming soon. Contact administrator for assistance.", 
                 GTK_MESSAGE_INFO);
}

static GtkWidget* create_login_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 40);
    gtk_widget_set_margin_bottom(page, 40);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size='xx-large' weight='bold'>Campus Management System</span>");
    gtk_box_append(GTK_BOX(page), title);
    
    GtkWidget *subtitle = gtk_label_new("Login to your account");
    gtk_widget_add_css_class(subtitle, "dim-label");
    gtk_box_append(GTK_BOX(page), subtitle);
    
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(form, 350, -1);
    
    GtkWidget *user_label = gtk_label_new("User ID");
    gtk_widget_set_halign(user_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), user_label);
    app_data->login_user_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->login_user_entry), "Enter your User ID");
    gtk_box_append(GTK_BOX(form), app_data->login_user_entry);
    
    GtkWidget *mobile_label = gtk_label_new("Mobile Number");
    gtk_widget_set_halign(mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), mobile_label);
    app_data->login_mobile_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->login_mobile_entry), "Enter 10-digit mobile");
    gtk_box_append(GTK_BOX(form), app_data->login_mobile_entry);
    
    GtkWidget *pass_label = gtk_label_new("Password");
    gtk_widget_set_halign(pass_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), pass_label);
    app_data->login_password_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->login_password_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->login_password_entry);
    
    app_data->login_status_label = gtk_label_new("");
    gtk_widget_add_css_class(app_data->login_status_label, "error");
    gtk_box_append(GTK_BOX(form), app_data->login_status_label);
    
    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(login_btn, "suggested-action");
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_box_append(GTK_BOX(form), login_btn);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(form), sep);
    
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(register_box, GTK_ALIGN_CENTER);
    GtkWidget *no_account = gtk_label_new("Don't have an account?");
    gtk_box_append(GTK_BOX(register_box), no_account);
    GtkWidget *register_link = gtk_button_new_with_label("Register");
    gtk_widget_add_css_class(register_link, "flat");
    g_signal_connect(register_link, "clicked", G_CALLBACK(on_goto_register), NULL);
    gtk_box_append(GTK_BOX(register_box), register_link);
    gtk_box_append(GTK_BOX(form), register_box);
    
    gtk_box_append(GTK_BOX(page), form);
    return page;
}

static GtkWidget* create_register_page(void) {
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 40);
    gtk_widget_set_margin_bottom(page, 40);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large' weight='bold'>Create New Account</span>");
    gtk_box_append(GTK_BOX(page), title);
    
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(form, 400, -1);
    
    GtkWidget *name_label = gtk_label_new("Full Name");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), name_label);
    app_data->reg_name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_name_entry), "Enter your full name");
    gtk_box_append(GTK_BOX(form), app_data->reg_name_entry);
    
    GtkWidget *email_label = gtk_label_new("Email Address");
    gtk_widget_set_halign(email_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), email_label);
    app_data->reg_email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_email_entry), "Enter your email");
    gtk_box_append(GTK_BOX(form), app_data->reg_email_entry);
    
    GtkWidget *mobile_label = gtk_label_new("Mobile Number");
    gtk_widget_set_halign(mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), mobile_label);
    app_data->reg_mobile_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_mobile_entry), "Enter 10-digit mobile");
    gtk_box_append(GTK_BOX(form), app_data->reg_mobile_entry);
    
    GtkWidget *campus_label = gtk_label_new("Campus Type");
    gtk_widget_set_halign(campus_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), campus_label);
    const char *campus_options[] = {"School", "College", "Hospital", "Hostel", NULL};
    app_data->reg_campus_combo = gtk_drop_down_new_from_strings(campus_options);
    gtk_box_append(GTK_BOX(form), app_data->reg_campus_combo);
    
    GtkWidget *pass_label = gtk_label_new("Password");
    gtk_widget_set_halign(pass_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), pass_label);
    app_data->reg_password_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->reg_password_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->reg_password_entry);
    
    GtkWidget *hint = gtk_label_new("Min 8 chars with uppercase, lowercase, and digit");
    gtk_widget_add_css_class(hint, "dim-label");
    gtk_widget_set_halign(hint, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), hint);
    
    GtkWidget *confirm_label = gtk_label_new("Confirm Password");
    gtk_widget_set_halign(confirm_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), confirm_label);
    app_data->reg_confirm_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->reg_confirm_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->reg_confirm_entry);
    
    app_data->reg_status_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(form), app_data->reg_status_label);
    
    GtkWidget *register_btn = gtk_button_new_with_label("Create Account");
    gtk_widget_add_css_class(register_btn, "suggested-action");
    g_signal_connect(register_btn, "clicked", G_CALLBACK(on_register_clicked), NULL);
    gtk_box_append(GTK_BOX(form), register_btn);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(form), sep);
    
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(login_box, GTK_ALIGN_CENTER);
    GtkWidget *has_account = gtk_label_new("Already have an account?");
    gtk_box_append(GTK_BOX(login_box), has_account);
    GtkWidget *login_link = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(login_link, "flat");
    g_signal_connect(login_link, "clicked", G_CALLBACK(on_goto_login), NULL);
    gtk_box_append(GTK_BOX(login_box), login_link);
    gtk_box_append(GTK_BOX(form), login_box);
    
    gtk_box_append(GTK_BOX(page), form);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), page);
    return scroll;
}

static GtkWidget* create_dashboard_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_start(page, 30);
    gtk_widget_set_margin_end(page, 30);
    gtk_widget_set_margin_top(page, 30);
    gtk_widget_set_margin_bottom(page, 30);
    
    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_hexpand(info_box, TRUE);
    
    app_data->dash_welcome_label = gtk_label_new("Welcome!");
    gtk_label_set_markup(GTK_LABEL(app_data->dash_welcome_label), 
                        "<span size='x-large' weight='bold'>Welcome!</span>");
    gtk_widget_set_halign(app_data->dash_welcome_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_welcome_label);
    
    app_data->dash_user_id_label = gtk_label_new("User ID: ");
    gtk_widget_set_halign(app_data->dash_user_id_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_user_id_label);
    
    app_data->dash_campus_label = gtk_label_new("Campus: ");
    gtk_widget_set_halign(app_data->dash_campus_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_campus_label);
    
    app_data->dash_email_label = gtk_label_new("Email: ");
    gtk_widget_set_halign(app_data->dash_email_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_email_label);
    
    app_data->dash_mobile_label = gtk_label_new("Mobile: ");
    gtk_widget_set_halign(app_data->dash_mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_mobile_label);
    
    gtk_box_append(GTK_BOX(header), info_box);
    
    GtkWidget *logout_btn = gtk_button_new_with_label("Logout");
    gtk_widget_add_css_class(logout_btn, "destructive-action");
    gtk_widget_set_valign(logout_btn, GTK_ALIGN_START);
    g_signal_connect(logout_btn, "clicked", G_CALLBACK(on_logout_clicked), NULL);
    gtk_box_append(GTK_BOX(header), logout_btn);
    
    gtk_box_append(GTK_BOX(page), header);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(page), sep);
    
    GtkWidget *actions_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(actions_label), "<span weight='bold'>Quick Actions</span>");
    gtk_widget_set_halign(actions_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(page), actions_label);
    
    GtkWidget *actions_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(actions_grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(actions_grid), 12);
    gtk_widget_set_halign(actions_grid, GTK_ALIGN_START);
    
    GtkWidget *view_profile_btn = gtk_button_new_with_label("View Profile");
    gtk_widget_set_size_request(view_profile_btn, 150, 60);
    g_signal_connect(view_profile_btn, "clicked", G_CALLBACK(on_view_profile_clicked), NULL);
    gtk_grid_attach(GTK_GRID(actions_grid), view_profile_btn, 0, 0, 1, 1);
    
    GtkWidget *edit_profile_btn = gtk_button_new_with_label("Edit Profile");
    gtk_widget_set_size_request(edit_profile_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), edit_profile_btn, 1, 0, 1, 1);
    
    GtkWidget *add_data_btn = gtk_button_new_with_label("Add Data");
    gtk_widget_set_size_request(add_data_btn, 150, 60);
    g_signal_connect(add_data_btn, "clicked", G_CALLBACK(on_add_data_clicked), NULL);
    gtk_grid_attach(GTK_GRID(actions_grid), add_data_btn, 2, 0, 1, 1);
    
    GtkWidget *view_data_btn = gtk_button_new_with_label("View Data");
    gtk_widget_set_size_request(view_data_btn, 150, 60);
    g_signal_connect(view_data_btn, "clicked", G_CALLBACK(on_view_data_clicked), NULL);
    gtk_grid_attach(GTK_GRID(actions_grid), view_data_btn, 0, 1, 1, 1);
    
    GtkWidget *export_pdf_btn = gtk_button_new_with_label("Export Report");
    gtk_widget_set_size_request(export_pdf_btn, 150, 60);
    g_signal_connect(export_pdf_btn, "clicked", G_CALLBACK(on_export_pdf_clicked), NULL);
    gtk_grid_attach(GTK_GRID(actions_grid), export_pdf_btn, 1, 1, 1, 1);
    
    GtkWidget *change_pass_btn = gtk_button_new_with_label("Change Password");
    gtk_widget_set_size_request(change_pass_btn, 150, 60);
    g_signal_connect(change_pass_btn, "clicked", G_CALLBACK(on_change_password_clicked), NULL);
    gtk_grid_attach(GTK_GRID(actions_grid), change_pass_btn, 2, 1, 1, 1);
    
    gtk_box_append(GTK_BOX(page), actions_grid);
    
    GtkWidget *status_frame = gtk_frame_new("System Status");
    gtk_widget_set_margin_top(status_frame, 20);
    GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_start(status_box, 12);
    gtk_widget_set_margin_end(status_box, 12);
    gtk_widget_set_margin_top(status_box, 12);
    gtk_widget_set_margin_bottom(status_box, 12);
    
    GtkWidget *db_status = gtk_label_new("Database: Connected");
    gtk_widget_set_halign(db_status, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), db_status);
    
    GtkWidget *session_status = gtk_label_new("Session: Active");
    gtk_widget_set_halign(session_status, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), session_status);
    
    char version_text[64];
    snprintf(version_text, sizeof(version_text), "Version: %s", APP_VERSION);
    GtkWidget *version_label = gtk_label_new(version_text);
    gtk_widget_set_halign(version_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), version_label);
    
    gtk_frame_set_child(GTK_FRAME(status_frame), status_box);
    gtk_box_append(GTK_BOX(page), status_frame);
    
    return page;
}

static GtkWidget* create_data_entry_page(void) {
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 20);
    gtk_widget_set_margin_bottom(page, 20);
    
    GtkWidget *back_btn = gtk_button_new_with_label("Back to Dashboard");
    gtk_widget_add_css_class(back_btn, "flat");
    gtk_widget_set_halign(back_btn, GTK_ALIGN_START);
    g_signal_connect(back_btn, "clicked", G_CALLBACK(on_back_to_dashboard), NULL);
    gtk_box_append(GTK_BOX(page), back_btn);
    
    app_data->data_form_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_box_append(GTK_BOX(page), app_data->data_form_container);
    
    app_data->data_status_label = gtk_label_new("");
    gtk_widget_set_margin_top(app_data->data_status_label, 12);
    gtk_box_append(GTK_BOX(page), app_data->data_status_label);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), page);
    return scroll;
}

static GtkWidget* create_view_data_page(void) {
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 20);
    gtk_widget_set_margin_bottom(page, 20);
    
    GtkWidget *back_btn = gtk_button_new_with_label("Back to Dashboard");
    gtk_widget_add_css_class(back_btn, "flat");
    gtk_widget_set_halign(back_btn, GTK_ALIGN_START);
    g_signal_connect(back_btn, "clicked", G_CALLBACK(on_back_to_dashboard), NULL);
    gtk_box_append(GTK_BOX(page), back_btn);
    
    app_data->view_data_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(app_data->view_data_content, 400, -1);
    gtk_box_append(GTK_BOX(page), app_data->view_data_content);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), page);
    return scroll;
}

static void activate(GtkApplication *app, gpointer user_data) {
    app_data = g_malloc0(sizeof(AppData));
    app_data->app = app;
    
    init_database();
    
    app_data->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_data->window), APP_NAME);
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 900, 700);
    
    app_data->header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar), 
                                    gtk_label_new(APP_NAME));
    gtk_window_set_titlebar(GTK_WINDOW(app_data->window), app_data->header_bar);
    
    app_data->stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(app_data->stack), 
                                  GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(app_data->stack), 300);
    
    app_data->login_page = create_login_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->login_page, "login");
    
    app_data->register_page = create_register_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->register_page, "register");
    
    app_data->dashboard_page = create_dashboard_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->dashboard_page, "dashboard");
    
    app_data->data_entry_page = create_data_entry_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->data_entry_page, "data_entry");
    
    app_data->view_data_page = create_view_data_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->view_data_page, "view_data");
    
    gtk_window_set_child(GTK_WINDOW(app_data->window), app_data->stack);
    
    gtk_window_present(GTK_WINDOW(app_data->window));
    
    log_activity("SYSTEM", "APP_STARTED", "Campus Management System GUI started");
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.campus.management", 
                                               G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    if (app_data) {
        close_database();
        g_free(app_data);
    }
    
    return status;
}
