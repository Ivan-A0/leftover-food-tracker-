document.addEventListener("DOMContentLoaded", function () {
    $.get("/get_leftover_report", function (data) {
        console.log("On page load, get leftover report from server.");
        console.log(data);

        populateReportCell("#leftover_food_list", data.most_common_leftover_);
        populateReportCell("#most_leftover_producing_meal_", data.most_leftover_producing_meal_);

        

        document.querySelector("#total_cost_of_leftover_").innerText = "$ " + data.total_cost_of_leftover_;

        populateReportCell("#most_common_leftover_reason_", data.most_common_leftover_reason_);

        populateReportCell("#most_common_disposal_mechanism_", data.most_common_disposal_mechanism_);

        populateReportCell("#suggested_strategies_to_reduce_leftover_", data.suggested_strategies_to_reduce_leftover_);

    });
});

function populateReportCell(list_id, data_array) {
    let el = document.querySelector(list_id);
    for (let i = 0; i < data_array.length; i++) {
        addListElement(el, data_array[i]);
    }
}

function addListElement(parent_element, list_string) {
    let li = document.createElement('li');
    li.innerText = list_string;
    parent_element.appendChild(li);    
}