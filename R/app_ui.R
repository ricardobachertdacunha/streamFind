#' The application User-Interface
#'
#' @param request Internal parameter for `{shiny}`.
#'     DO NOT REMOVE.
#' @import shiny
#' @noRd
app_ui <- function(request) {
  tagList(
    # Leave this function for adding external resources
    golem_add_external_resources(),
    # Your application UI logic
    fluidPage(
      h1("streamFind"),
      html_print(tagList(
        tags$p("You can drag and drop the diagrams to switch order:"),
        tags$div(
          id = "aUniqueId",
          tags$div(
            style = "border: solid 0.2em gray; float:left; margin: 5px",
            mermaid("graph LR; S[SortableJS] -->|sortable| R ",
                    height = 250, width = 300)
          ),
          tags$div(
            style = "border: solid 0.2em gray; float:left; margin: 5px",
            mermaid("graph TD; JavaScript -->|htmlwidgets| R ",
                    height = 250, width = 150)
          )
        ),
        sortable_js("aUniqueId") # the CSS id
      ))
    )
  )
}

#' Add external Resources to the Application
#'
#' This function is internally used to add external
#' resources inside the Shiny application.
#'
#' @import shiny
#' @importFrom golem add_resource_path activate_js favicon bundle_resources
#' @noRd
golem_add_external_resources <- function() {
  add_resource_path(
    "www",
    app_sys("app/www")
  )

  tags$head(
    favicon(ext = "png"),
    bundle_resources(
      path = app_sys("app/www"),
      app_title = "streamFind"
    )
    # Add here other external resources
    # for example, you can add shinyalert::useShinyalert()
  )
}
